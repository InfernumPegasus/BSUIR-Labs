#ifndef LAB3_9_MATRIX_H
#define LAB3_9_MATRIX_H

#include <iostream>
const int c_outOfRange = 3;

class Dimension
{
private:
    int rows;
    int cols;
public:
    int getRows() const
    { return rows; }
    int getCols() const
    { return cols; }
    void setRows(int r)
    { rows = r;}
    void setCols(int c)
    { cols = c;}

    void Show() const
    {
        std::cout << "Rows: " << rows << "\n";
        std::cout << "Cols: " << cols;
    }
};

class Matrix
{
private:
    int m_rows;
    int m_columns;
    int **m_matrix{};

    void Clear();
    void MemAlloc();
public:
    explicit Matrix(const int rows = 2, const int columns = 2)
    : m_rows(rows), m_columns(columns)
    {
        MemAlloc();
    }

    Matrix(const Matrix& supply)
    : m_rows(supply.m_rows), m_columns(supply.m_columns)
    {
        MemAlloc();
        for (int i = 0; i < m_rows; i++)
            for (int j = 0; j < m_columns; j++)
                m_matrix[i][j] = supply.m_matrix[i][j];
    }

    ~Matrix()
    {
//        std::cout << "Destructor in Matrix!\n";
        Clear();
    }

    // ДОП ЗАДАНИЕ
    static Matrix biggestMatrix(Matrix& m1, Matrix& m2);

    void RandInit();
    friend int** getMatrix(Matrix& matrix);
    friend Dimension getDimensions(const Matrix& matrix);
    friend int elementsNumber(Matrix& matrix);

    // перегрузка оператора == для сравнения размеров матриц
    bool operator== (Matrix& m) const;
    // оператор + складывает матрицы одной размерности
    Matrix& operator+ (Matrix& m);
    // оператор индексации для целых чисел []
    int* operator[] (int index);

    friend bool operator< (Matrix& m1, Matrix& m2);
    friend Matrix operator+ (const Matrix& m, int value);
    friend Matrix operator-- (Matrix& m);
    friend std::ostream& operator<< (std::ostream &out, const Matrix& m);
    int& operator() (int row, int col);
    Matrix& operator= (const Matrix& m);
    Matrix& operator++ ();
    friend Matrix operator++ (Matrix& m, int);
};

#endif //LAB3_9_MATRIX_H
