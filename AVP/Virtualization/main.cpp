#include <iostream>
#include <vector>
#include <random>
#include <chrono>

using std::size_t;
using std::vector;

using std::cout;
using std::endl;

using RequiredType = double;
using Submatrix = RequiredType **;
using Matrix = Submatrix **;

std::random_device randomDevice;
std::mt19937 gen(randomDevice());
std::uniform_int_distribution<> distribution(0, 100);

class TMatrix {
public:
    TMatrix(size_t rows, size_t columns) : rows_(rows), columns_(columns) {
        matrix_ = new Submatrix *[rows_];
        for (int i {0}; i < rows_; i++) {
            matrix_[i] = new Submatrix[columns_];
            for (int j {0}; j < columns_; j++) {
                matrix_[i][j] = new RequiredType *[NESTED_MATRIX_ROWS];
                for (int k {0}; k < NESTED_MATRIX_ROWS; k++) {
                    matrix_[i][j][k] = new RequiredType[NESTED_MATRIX_COLUMNS];
                }
            }
        }
        cout << "Object constructed.\n";
    }

    ~TMatrix() {
        for (int i{0}; i < rows_; i++) {
            delete [] matrix_[i];
        }
        delete [] matrix_;
        cout << "Object destructed.\n";
    }

public:
    TMatrix Multiply(const TMatrix& rhs) {
        auto rows1 = rows_;
        auto columns1 = columns_;
        auto rows2 = rhs.rows_;
        auto columns2 = rhs.columns_;

        if (rows2 != columns1) {
            throw std::out_of_range("Rows and columns of matrices are not equal!");
        }

        TMatrix temp(rows1, columns2);
//        cout << "rows1: " << rows1 << " columns1: " << columns1
//        << " rows2: " << rows2 << " columns: " << columns2 << endl;

        Submatrix res;
        res = new RequiredType *[NESTED_MATRIX_ROWS];
        for (int i {0}; i < NESTED_MATRIX_ROWS; i++) {
            res[i] = new RequiredType[NESTED_MATRIX_COLUMNS];
        }

        for (int i {0}; i < rows1; i++) {
            if (i % 100 == 0)
                cout << "Processing " << i << " of " << rows1 << endl;
            for (int j{0}; j < columns2; j++)
                for (int k{0}; k < rows2; k++)
                    temp.matrix_[i][j] = MultiplySubmatrix(matrix_[i][k], rhs.matrix_[k][j], res);
        }

        return temp;
    }

    static Submatrix MultiplySubmatrix(const Submatrix &m1,
                                       const Submatrix &m2,
                                       Submatrix &res) {
        for (int i {0}; i < NESTED_MATRIX_ROWS; i++)
            for (int j {0}; j < NESTED_MATRIX_COLUMNS; j++)
                for (int k {0}; k < NESTED_MATRIX_ROWS; k++)
                    res[i][j] += m1[i][k] * m2[k][j];

        return res;
    }

    bool operator==(const TMatrix &rhs) const {
        if (rows_ != rhs.rows_ ||
            columns_ != rhs.columns_) {
            return false;
        }
        for (size_t i = 0; i < rhs.rows_; i++)
            for (size_t j = 0; j < rhs.columns_; j++)
                for (size_t k = 0; k < NESTED_MATRIX_ROWS; k++)
                    for (size_t l = 0; l < NESTED_MATRIX_COLUMNS; l++)
                        if (rhs.matrix_[i][j][k][l] != matrix_[i][j][k][l])
                            return false;
        return true;
    }

public:
    [[nodiscard]] size_t GetRows() const {
        return rows_;
    }

    [[nodiscard]] size_t GetColumns() const {
        return columns_;
    }

    void Print() const {
        for (size_t i = 0; i < rows_; i++)
            for (size_t j = 0; j < columns_; j++)
                PrintSubmatrix(matrix_[i][j]);
    }

    void FillRandom() {
        for (size_t i = 0; i < rows_; i++)
            for (size_t j = 0; j < columns_; j++)
                FillSubmatrixRandom(matrix_[i][j]);
    }

private:
    static void FillSubmatrixRandom(Submatrix &submatrix) {
        for (size_t i = 0; i < NESTED_MATRIX_ROWS; i++) {
            for (size_t j = 0; j < NESTED_MATRIX_COLUMNS; j++) {
                submatrix[i][j] = distribution(gen);
            }
        }
    }
    static void PrintSubmatrix(const Submatrix &submatrix) {
        printf("[\n");
        for (size_t i = 0; i < NESTED_MATRIX_ROWS; i++) {
            for (size_t j = 0; j < NESTED_MATRIX_COLUMNS; j++) {
                std::cout << submatrix[i][j] << "\t";
            }
            std::cout << "\n";
        }
        printf("]\n");
    }

public:
    static constexpr auto NESTED_MATRIX_ROWS {4};
    static constexpr auto NESTED_MATRIX_COLUMNS {3};

private:
    size_t rows_;
    size_t columns_;

    Matrix matrix_;
};

int main() {
    TMatrix tMatrix1(300, 300);
    TMatrix tMatrix2(300, 300);

    cout << "T1: " << tMatrix1.GetRows() << " " << tMatrix1.GetColumns() << endl;
    tMatrix1.FillRandom();
//    tMatrix1.Print();
//    cout << endl;
    cout << "T2: " << tMatrix2.GetRows() << " " << tMatrix2.GetColumns() << endl;
    tMatrix2.FillRandom();
//    tMatrix2.Print();
//    cout << endl;

    auto start = std::chrono::high_resolution_clock::now();
    auto multiplied = tMatrix1.Multiply(tMatrix2);
    auto end = std::chrono::high_resolution_clock::now();
    auto dif = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "Time : " << dif << endl;

    cout << "T3: " << multiplied.GetRows() << " " << multiplied.GetColumns() << endl;
//    multiplied.Print();

    return 0;
}
