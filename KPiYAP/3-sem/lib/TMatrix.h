//
// Created by USER on 24.09.2021.
//

#ifndef LAB5_9_TMATRIX_H
#define LAB5_9_TMATRIX_H

template<class T>
class Matrix
{
private:
    T **m_matrix;
    int m_rows;
    int m_columns;

    void MemAlloc()
    {
        m_matrix = (T **) new T*[m_rows];
        for (int i = 0; i < m_rows; i++)
            m_matrix[i] = (T *) new T[m_columns];
    }
public:
    // Default Constructor
    Matrix()
    {
        m_rows    = 0;
        m_columns = 0;
        m_matrix  = nullptr;
    }

    // Main constructor
    explicit Matrix(int rows, int columns)
    {
        m_rows    = rows;
        m_columns = columns;
        MemAlloc();
    }

    Matrix(const Matrix& source)
    {
        m_rows    = source.m_rows;
        m_columns = source.m_columns;
        MemAlloc();

        for (int i = 0; i < m_rows; i++)
            for (int j = 0; j < m_columns; j++)
                m_matrix[i][j] = source.m_matrix[i][j];
    }

    ~Matrix()
    {
        for (int i = 0; i < m_rows; i++)
            delete [] m_matrix[i];
        delete [] m_matrix;
    }

    // Get element
    T Get(int i, int j);

    // Erasing matrix
    void Erase();

    // Overloaded operator[]
    T& operator[](int index);
};




#endif //LAB5_9_TMATRIX_H
