#include <iostream>
#include <vector>
#include <random>
#include <chrono>

using std::size_t;
using std::vector;
using std::cout;

typedef double RequiredType;
using Submatrix = vector<vector<RequiredType>>;
using Matrix = vector<vector<Submatrix>>;

std::random_device randomDevice;
std::mt19937 gen(randomDevice());
std::uniform_int_distribution<> distribution(0, 100);

class TMatrix {
public:
    TMatrix(size_t rows, size_t columns) : rows_(rows), columns_(columns) {
        matrix_.resize(rows_);
        for (auto &row: matrix_) {
            row.resize(columns_);
            for (auto &col: row) {
                col.resize(NESTED_MATRIX_ROWS);
                for (auto &nRow: col) {
                    nRow.resize(NESTED_MATRIX_COLUMNS);
                }
            }
        }
    }

public:
    TMatrix *Multiply(const TMatrix &rhs) {
        if (rhs.rows_ != columns_) {
            throw std::out_of_range("Rows and columns of matrices are not equal!");
        }

        auto *res = new TMatrix(rows_, rhs.columns_);

        Submatrix submatrix(NESTED_MATRIX_ROWS);
        for (auto &nRow: submatrix) {
            nRow.resize(NESTED_MATRIX_COLUMNS);
        }

        for (int i{0}; i < rows_; i++) {
            if (i % 100 == 0)
                cout << "Processing " << i << " of " << rows_ << "\n";
            for (int j{0}; j < rhs.columns_; j++)
                for (int k{0}; k < rhs.rows_; k++) {
                    MultiplySubmatrix(matrix_[i][k], rhs.matrix_[k][j], submatrix);
                    AddSubmatrix(res->matrix_[i][j], submatrix, res->matrix_[i][j]);
                }
        }

        return res;
    }

    static void MultiplySubmatrix(const Submatrix &m1, const Submatrix &m2, Submatrix &res) {
        for (int i{0}; i < NESTED_MATRIX_ROWS; i++)
            for (int j{0}; j < NESTED_MATRIX_COLUMNS; j++)
                for (int k{0}; k < NESTED_MATRIX_ROWS; k++)
                    res[i][j] += m1[i][k] * m2[k][j];
    }

    static void AddSubmatrix(const Submatrix &m1, const Submatrix &m2, Submatrix &res) {
        for (int i{0}; i < NESTED_MATRIX_ROWS; i++)
            for (int j{0}; j < NESTED_MATRIX_COLUMNS; j++)
                res[i][j] = m1[i][j] + m2[i][j];
    }

    bool operator==(const TMatrix &rhs) const {
        if (rows_ != rhs.rows_ ||
            columns_ != rhs.columns_) {
            return false;
        }
        return matrix_ == rhs.matrix_;
    }

public:
    [[nodiscard]] size_t GetRows() const {
        return rows_;
    }

    [[nodiscard]] size_t GetColumns() const {
        return columns_;
    }

    void Print() const {
        for (int i{0}; i < rows_; i++)
            for (int j{0}; j < columns_; j++)
                PrintSubmatrix(matrix_[i][j]);
        cout << "\n";
    }

    void FillRandom() {
        for (int i{0}; i < rows_; i++)
            for (int j{0}; j < columns_; j++)
                FillSubmatrixRandom(matrix_[i][j]);
    }

private:
    static void FillSubmatrixRandom(Submatrix &submatrix) {
        for (int i{0}; i < NESTED_MATRIX_ROWS; i++) {
            for (int j{0}; j < NESTED_MATRIX_COLUMNS; j++) {
                submatrix[i][j] = distribution(gen);
            }
        }
    }

    static void PrintSubmatrix(const Submatrix &submatrix) {
        cout << "[\n";
        for (int i{0}; i < NESTED_MATRIX_ROWS; i++) {
            for (int j{0}; j < NESTED_MATRIX_COLUMNS; j++) {
                cout << submatrix[i][j] << "\t";
            }
            cout << "\n";
        }
        cout << "]\n";
    }

public:
    static constexpr auto NESTED_MATRIX_ROWS{4};
    static constexpr auto NESTED_MATRIX_COLUMNS{3};

private:
    size_t rows_;
    size_t columns_;

    Matrix matrix_;
};

int main() {
    {
        TMatrix tMatrix1(1000, 1000);
        TMatrix tMatrix2(1000, 1000);

        cout << "T1: " << tMatrix1.GetRows() << " " << tMatrix1.GetColumns() << "\n";
        tMatrix1.FillRandom();
//        tMatrix1.Print();
        cout << "T2: " << tMatrix2.GetRows() << " " << tMatrix2.GetColumns() << "\n";
        tMatrix2.FillRandom();
//        tMatrix2.Print();

        auto start = std::chrono::high_resolution_clock::now();
        auto multiplied = tMatrix1.Multiply(tMatrix2);
        auto end = std::chrono::high_resolution_clock::now();
        auto dif = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
        std::cout << "Time : " << dif << "\n";

        cout << "T3: " << multiplied->GetRows() << " " << multiplied->GetColumns() << "\n";
//        multiplied->Print();

        delete multiplied;
    }

    return 0;
}
