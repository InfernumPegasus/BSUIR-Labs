#include <iostream>
#include <vector>
#include <random>

using std::size_t;

template<class MatrixContentType>
class MatrixOperations {
public:
    virtual MatrixContentType& operator*=(const MatrixContentType& rhs) = 0;
};

template<class T>
class TMatrix;

using RequiredType = double;
using NestedMatrixType = TMatrix<RequiredType>;

template<>
class TMatrix<RequiredType> /*: public MatrixOperations<TMatrix<RequiredType>>*/ {
public:
    TMatrix(size_t rows, size_t columns) : rows_(rows), columns_(columns), matrix_(rows_) {
        for (auto &row: matrix_)
            row.resize(columns_);
    }

    ~TMatrix() = default;

public:
    TMatrix &operator*=(const TMatrix &rhs) {
        if (rhs.rows_ != columns_) {
            throw std::out_of_range("Rows and columns of matrices are not equal!");
        }

        TMatrix temp(rows_, rhs.columns_);

        for (size_t i = 0; i < temp.rows_; i++)
            for (size_t j = 0; j < temp.columns_; j++)
                for (size_t k = 0; k < columns_; k++)
                    temp[i][j] += matrix_[i][k] * rhs[k][j];

        std::swap(matrix_, temp.matrix_);
        std::swap(columns_, temp.columns_);

        return *this;
    }

    std::vector<RequiredType> &operator[](size_t i) {
        return matrix_[i];
    }

    const std::vector<RequiredType> &operator[](size_t i) const {
        return matrix_[i];
    }

    TMatrix operator*(const TMatrix &rhs) {
        return *this *= rhs;
    }

    bool operator==(const TMatrix &rhs) const {
        return rows_ == rhs.rows_ &&
               columns_ == rhs.columns_ &&
               matrix_ == rhs.matrix_;
    }

public:
    [[nodiscard]] size_t GetRows() const {
        return rows_;
    }

    [[nodiscard]] size_t GetColumns() const {
        return columns_;
    }

    void FillRandom() {
        std::random_device randomDevice;
        std::mt19937 gen(randomDevice());
        std::uniform_int_distribution<> distribution(0, 100);

        for (size_t i = 0; i < rows_; i++) {
            for (size_t j = 0; j < columns_; j++) {
                matrix_[i][j] = distribution(gen);
            }
        }
    }

    void Print() const {
        for (size_t i = 0; i < rows_; i++) {
            for (size_t j = 0; j < columns_; j++) {
                std::cout << matrix_[i][j] << "\t";
            }
            std::cout << std::endl;
        }
    }

private:
    size_t rows_;
    size_t columns_;

    std::vector<std::vector<RequiredType>> matrix_;
};

void f(TMatrix<RequiredType> &matrix, const TMatrix<RequiredType> &other) {
    auto a = matrix * other;
}

int main() {
    using std::cout;
    using std::endl;

    TMatrix<RequiredType> tMatrix1(23, 12);
    TMatrix<RequiredType> tMatrix2(12, 4);

    cout << "T1: " << tMatrix1.GetRows() << " " << tMatrix1.GetColumns() << endl;
    tMatrix1.FillRandom();
    tMatrix1.Print();
    cout << "T2: " << tMatrix2.GetRows() << " " << tMatrix2.GetColumns() << endl;
    tMatrix2.FillRandom();
    tMatrix2.Print();

    auto multiplied = tMatrix1 * tMatrix2;
    cout << "T3: " << multiplied.GetRows() << " " << multiplied.GetColumns() << endl;
    TMatrix<RequiredType> tMatrix3(23, 4);
    multiplied.Print();
//    cout << std::boolalpha << (tMatrix3 == multiplied) << endl;

    return 0;
}
