//
// Created by USER on 09.09.2021.
//

#include "Matrix.h"

// функция подсчета ячеек матрицы
int elementsNumber(Matrix& matrix)
{ return matrix.m_rows * matrix.m_columns; }

// перегрузка < (сравниваются размеры матриц)
bool operator< (Matrix& m1, Matrix& m2)
{ return (m1.m_rows * m1.m_columns < m2.m_rows * m2.m_columns); }

// перегрузка << для типа Matrix
std::ostream& operator<< (std::ostream &out, const Matrix& m)
{
    for (int i = 0; i < m.m_rows; ++i)
        for (int j = 0; j < m.m_columns; ++j)
            out << "A[" << i+1 << "][" << j+1 << "] = " << m.m_matrix[i][j] << std::endl;

    return out;
}

// перегрузка -- префиксного
Matrix operator-- (Matrix& m)
{
    for (int i = 0; i < m.m_rows; ++i)
        for (int j = 0; j < m.m_columns; ++j)
            m.m_matrix[i][j]--;

    return m;
}

// перегруза + для int
Matrix operator+(const Matrix& m, int value)
{
    for (int i = 0; i < m.m_rows; ++i)
        for (int j = 0; j < m.m_columns; ++j)
            m.m_matrix[i][j] += value;
    return m;
}

// перегрузка оператора индексации []
int *Matrix::operator[](int index)
{ return m_matrix[index]; }

// оператор + складывает матрицы
Matrix &Matrix::operator+(Matrix &m)
{
    if (this->m_columns != m.m_columns || this->m_rows != m.m_rows)
        exit(c_outOfRange);

    for (int i = 0; i < m_rows; ++i)
        for (int j = 0; j < m_columns; ++j)
            m_matrix[i][j] += m.m_matrix[i][j];
    return *this;
}

// инициализация матрицы рандомными числами
void Matrix::RandInit()
{
    for (int i = 0; i < m_rows; ++i)
    {
        for (int j = 0; j < m_columns; ++j)
            m_matrix[i][j] = rand() % 100;
    }
}

// функция преобразования Matrix в int**
int** getMatrix(Matrix& matrix)
{ return matrix.m_matrix; }

// оператор инициализации ()
int &Matrix::operator()(int row, int col)
{
    if (row < 0 || row >= m_rows || col < 0 || col > m_columns)
        std::exit(c_outOfRange);

    return m_matrix[row][col];
}

// оператор присваивания =
Matrix &Matrix::operator=(const Matrix &m)
{
    // проверка самоприсвоения
    if (this != std::addressof(m))
    {
        // копирование размеров
        m_rows = m.m_rows;
        m_columns = m.m_columns;

        // выделение памяти
        m_matrix = new int*[m_rows];
        for (int i = 0; i < m_rows; ++i)
            m_matrix[i] = new int[m_columns];

        // копирование
        for (int i = 0; i < m_rows; ++i)
            for (int j = 0; j < m_columns; ++j)
                m_matrix[i][j] = m.m_matrix[i][j];
    }
    return *this;
}

// префиксный оператор ++
Matrix &Matrix::operator++()
{
    for (int i = 0; i < m_rows; ++i)
        for (int j = 0; j < m_columns; ++j)
            ++m_matrix[i][j];

    return *this;
}

// перегрузка оператора == для сравнения размеров матриц
bool Matrix::operator==(Matrix &m) const
{ return (m_rows == m.m_rows && m_columns == m.m_columns); }

// удаление матрицы
void Matrix::Clear()
{
    for (int i = 0; i < m_rows; ++i)
        delete [] m_matrix[i];
    delete [] m_matrix;
    m_matrix = nullptr;
}

// выделение памяти под матрицу
void Matrix::MemAlloc()
{
    m_matrix  = new int*[m_rows];

    for (int i = 0; i < m_rows; ++i)
        m_matrix[i] = new int[m_columns];
}

// преобразование Matrix в тип Dimension
Dimension getDimensions(const Matrix &matrix)
{
    Dimension dims{};
    dims.setCols(matrix.m_columns);
    dims.setRows(matrix.m_rows);
    return dims;
}

Matrix Matrix::biggestMatrix(Matrix &m1, Matrix &m2)
{
    if (m1.m_rows != m2.m_rows && m1.m_columns != m2.m_columns)
    {
        std::cout << "Bad size!\n";
        exit(2);
    }

    int rows = m1.m_rows;
    int cols = m1.m_columns;

    Matrix result(rows, cols);

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            --m1;
            m2++;

            if (m1[i][j] > m2[i][j])
                result[i][j] = m1[i][j];
            else
                result[i][j] = m2[i][j];
        }
    }

    return result;
}

Matrix operator++(Matrix &m, int)
{
    Matrix old(m);
    for (int i = 0; i < old.m_rows; i++)
        for (int j = 0; j < old.m_columns; j++)
            old[i][j]++;
    return old;
}


// перегрузка << для типа Dimension
std::ostream &operator<<(std::ostream &out, const Dimension &dim)
{
    out << "Rows :" << dim.getRows() <<  "\n" << "Cols :" << dim.getCols() <<  "\n";
    return out;
}


