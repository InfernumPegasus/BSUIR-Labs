//
// Created by USER on 24.09.2021.
//

#include "TMatrix.h"

template<class T>
void Matrix<T>::Erase()
{
    for (int i = 0; i < m_rows; i++)
        delete [] m_matrix[i];
    delete [] m_matrix;
}

template<class T>
T &Matrix<T>::operator[](int index)
{ return m_matrix[index]; }

//template<class T>
//T Matrix::Get(int i, int j)
//{ return m_matrix[i][j]; }