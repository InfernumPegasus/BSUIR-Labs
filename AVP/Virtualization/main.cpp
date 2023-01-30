#include <iostream>
#include <vector>
#include <random>
#include <chrono>
//#include <execution>
#include <emmintrin.h>

#ifdef __SSE2__
#include <emmintrin.h>
#else
#warning SSE2 support is not available. Code will not compile
#endif

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
                for (int k{0}; k < rhs.rows_; k++) {
                    NoVectorizedMultiplySubmatrix(matrix_[i][k], rhs.matrix_[k][j], submatrix);
                    NoVectorizedAddSubmatrix(res->matrix_[i][j], submatrix, res->matrix_[i][j]);
                }
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

    [[gnu::target("no-sse")]]
    static void NoVectorizedAddSubmatrix(const Submatrix &m1, const Submatrix &m2, Submatrix &res) {
        for (int i{0}; i < NESTED_MATRIX_ROWS; i++)
            for (int j{0}; j < NESTED_MATRIX_COLUMNS; j++)
                res[i][j] = m1[i][j] + m2[i][j];
    }

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
                for (int k{0}; k < rhs.rows_; k++) {
                    AutoVectorizedMultiplySubmatrix(matrix_[i][k], rhs.matrix_[k][j], submatrix);
                    AutoVectorizedAddSubmatrix(res->matrix_[i][j], submatrix, res->matrix_[i][j]);
                }
        }

        return res;
    }

    static void AutoVectorizedMultiplySubmatrix(const Submatrix &m1, const Submatrix &m2, Submatrix &res) {
        for (int i{0}; i < NESTED_MATRIX_ROWS; i++)
            for (int j{0}; j < NESTED_MATRIX_COLUMNS; j++)
                for (int k{0}; k < NESTED_MATRIX_ROWS; k++)
                    res[i][j] += m1[i][k] * m2[k][j];
    }

    static void AutoVectorizedAddSubmatrix(const Submatrix &m1, const Submatrix &m2, Submatrix &res) {
        for (int i{0}; i < NESTED_MATRIX_ROWS; i++)
            for (int j{0}; j < NESTED_MATRIX_COLUMNS; j++)
                res[i][j] = m1[i][j] + m2[i][j];
    }

public:
    bool operator==(const TMatrix &rhs) const {
        if (rows_ != rhs.rows_ ||
            columns_ != rhs.columns_) {
            return false;
        }
        return matrix_ == rhs.matrix_;
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
    static constexpr auto NESTED_MATRIX_ROWS{4};
    static constexpr auto NESTED_MATRIX_COLUMNS{3};

private:
    size_t rows_;
    size_t columns_;

    Matrix matrix_;
};

int N = 5;
void mult(int **mat1, int **mat2, int **result) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j += 8) {
            auto sum = _mm_setzero_si128();
            for (int k = 0; k < N; k++) {
                auto bc_mat1 = _mm_set1_epi32(mat1[i][k]);
                auto vec_mat2 = _mm_loadu_si16((__m128i*)&mat2[k][j]);
                auto prod = _mm_mullo_epi16(bc_mat1, vec_mat2);
                sum = _mm_add_epi32(sum, prod);
            }
            _mm_storeu_si16((__m128i*)&result[i][j], sum);
        }
    }
}

int main() {
//    {
//        TMatrix tMatrix1(300, 300);
//        TMatrix tMatrix2(300, 300);
//
//        cout << "T1: " << tMatrix1.GetRows() << " " << tMatrix1.GetColumns() << "\n";
//        tMatrix1.FillRandom();
////        tMatrix1.Print();
//        cout << "T2: " << tMatrix2.GetRows() << " " << tMatrix2.GetColumns() << "\n";
//        tMatrix2.FillRandom();
////        tMatrix2.Print();
//
//        auto start = std::chrono::high_resolution_clock::now();
//        auto multiplied = tMatrix1.AutoVectorizedMultiply(tMatrix2);
//        auto end = std::chrono::high_resolution_clock::now();
//        auto dif = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
//        std::cout << "Time : " << dif << "\n";
//
//        cout << "T3: " << multiplied->GetRows() << " " << multiplied->GetColumns() << "\n";
////        multiplied->Print();
//
//        start = std::chrono::high_resolution_clock::now();
//        multiplied = tMatrix1.NoVectorizedMultiply(tMatrix2);
//        end = std::chrono::high_resolution_clock::now();
//        dif = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
//        std::cout << "Time : " << dif << "\n";
//
//        cout << "T3: " << multiplied->GetRows() << " " << multiplied->GetColumns() << "\n";
//
//        delete multiplied;
//    }

    {
        int **array1 = new int *[N];
        for (int i = 0; i < N; i++) {
            array1[i] = new int[N];
        }
        int **array2 = new int *[N];
        for (int i = 0; i < N; i++) {
            array2[i] = new int[N];
        }
        int **array3 = new int *[N];
        for (int i = 0; i < N; i++) {
            array3[i] = new int[N];
        }

        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                array1[i][j] = 1;
                array2[i][j] = 2;
//                array3[i][j] = 3;
            }
        }

        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                cout << array1[i][j] << "\t";
            }
            cout << "\n";
        }
        cout << "\n";

        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                cout << array2[i][j] << "\t";
            }
            cout << "\n";
        }
        cout << "\n";

        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                cout << array3[i][j] << "\t";
            }
            cout << "\n";
        }
        cout << "\n";

        mult(array1, array2, array3);

        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                cout << array3[i][j] << "\t";
            }
            cout << "\n";
        }
    }

    return 0;
}
