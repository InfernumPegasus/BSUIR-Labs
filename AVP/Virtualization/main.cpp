#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <immintrin.h>

using std::size_t;
using std::vector;
using std::cout;

using RequiredType = double;
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
    [[gnu::target("no-sse")]]
    TMatrix *NoVectorizedMultiply(const TMatrix &rhs) {
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
                for (int k{0}; k < rhs.rows_; k++)
                    NoVectorizedMultiplySubmatrix(matrix_[i][k], rhs.matrix_[k][j], submatrix);
        }

        return res;
    }

    [[gnu::target("no-sse")]]
    static void NoVectorizedMultiplySubmatrix(const Submatrix &m1, const Submatrix &m2, Submatrix &res) {
        for (int i{0}; i < NESTED_MATRIX_ROWS; i++)
            for (int j{0}; j < NESTED_MATRIX_COLUMNS; j++)
                for (int k{0}; k < NESTED_MATRIX_ROWS; k++)
                    res[i][j] += m1[i][k] * m2[k][j];
    }

    [[gnu::target("sse")]]
    TMatrix *AutoVectorizedMultiply(const TMatrix &rhs) {
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
                for (int k{0}; k < rhs.rows_; k++)
                    AutoVectorizedMultiplySubmatrix(matrix_[i][k], rhs.matrix_[k][j], submatrix);
        }

        return res;
    }

    [[gnu::target("sse")]]
    static void AutoVectorizedMultiplySubmatrix(const Submatrix &m1, const Submatrix &m2, Submatrix &res) {
        for (int i{0}; i < NESTED_MATRIX_ROWS; i++)
            for (int j{0}; j < NESTED_MATRIX_COLUMNS; j++)
                for (int k{0}; k < NESTED_MATRIX_ROWS; k++)
                    res[i][j] += m1[i][k] * m2[k][j];
    }

    TMatrix *VectorizedMultiply(const TMatrix &rhs) {
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
                for (int k{0}; k < rhs.rows_; k++)
                    VectorizedMultiplySubmatrix(matrix_[i][k], rhs.matrix_[k][j], submatrix);
        }

        return res;
    }

    static void VectorizedMultiplySubmatrix(const Submatrix &m1, const Submatrix &m2, Submatrix &res) {
        for (int i = 0; i < NESTED_MATRIX_ROWS; i++) {
            for (int j = 0; j < NESTED_MATRIX_COLUMNS; j += 8) {
                auto sum = _mm256_setzero_pd();
                for (int k = 0; k < NESTED_MATRIX_ROWS; k++) {
                    auto bc_mat1 = _mm256_set1_pd(m1[i][k]);
                    auto vec_mat2 = _mm256_loadu_pd(&m2[k][j]);
                    auto prod = _mm256_mul_pd(bc_mat1, vec_mat2);
                    sum = _mm256_add_pd(sum, prod);
                }
                _mm256_storeu_pd(&res[i][j], sum);
            }
        }
    }

public:
    bool operator==(const TMatrix &rhs) const {
        if (rows_ != rhs.rows_ ||
            columns_ != rhs.columns_) {
            return false;
        }
        for (int i{0}; i < rows_; i++)
            for (int j{0}; j < columns_; j++)
                for (int k{0}; k < NESTED_MATRIX_ROWS; k++)
                    for (int l{0}; l < NESTED_MATRIX_COLUMNS; l++)
                        if (matrix_[i][j][k][l] != rhs.matrix_[i][j][k][l]) return false;
        return true;

//        return matrix_ == rhs.matrix_;
    }

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
    static constexpr auto NESTED_MATRIX_ROWS{12};
    static constexpr auto NESTED_MATRIX_COLUMNS{12};

private:
    size_t rows_;
    size_t columns_;

    Matrix matrix_;
};

int main() {
    {
        TMatrix tMatrix1(100, 100);
        TMatrix tMatrix2(100, 100);
        tMatrix1.FillRandom();
        tMatrix2.FillRandom();

        cout << "Auto vectorization:\n";
        auto start = std::chrono::high_resolution_clock::now();
        auto multiplied1 = tMatrix1.AutoVectorizedMultiply(tMatrix2);
        auto end = std::chrono::high_resolution_clock::now();
        auto dif = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        std::cout << "Time : " << dif << " ms\n";
        cout << "T3: " << multiplied1->GetRows() << " " << multiplied1->GetColumns() << "\n\n";
//        multiplied1->Print();

        cout << "Vectorization:\n";
        start = std::chrono::high_resolution_clock::now();
        auto multiplied3 = tMatrix1.VectorizedMultiply(tMatrix2);
        end = std::chrono::high_resolution_clock::now();
        dif = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        std::cout << "Time : " << dif << " ms\n";
        cout << "T3: " << multiplied3->GetRows() << " " << multiplied3->GetColumns() << "\n\n";
//        multiplied3->Print();

        cout << "No vectorization:\n";
        start = std::chrono::high_resolution_clock::now();
        auto multiplied2 = tMatrix1.NoVectorizedMultiply(tMatrix2);
        end = std::chrono::high_resolution_clock::now();
        dif = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        std::cout << "Time : " << dif << " ms\n";
        cout << "T3: " << multiplied2->GetRows() << " " << multiplied2->GetColumns() << "\n\n";
//        multiplied2->Print();

        if (*multiplied1 == *multiplied2 && *multiplied1 == *multiplied3 && *multiplied2 == *multiplied3) {
            cout << "All matrices are equal.\n";
        } else {
            cout << "Matrices are not equal.\n";
        }

        delete multiplied1;
        delete multiplied2;
        delete multiplied3;
    }

    return 0;
}
