// Lab 3.9

#include "Matrix.h"
#include "D:\Development\C++\Projects\Labs\lib\Input.h"

// 1 матрица префиксно увеличивается
// 2 матрица префиксно уменьшается
// создать 3 матрицу в которую заносится больший жлеменет из двух матриц

int main()
{
    Matrix m1(3, 3);
    Matrix m2(3, 3);
    m1.RandInit();
    m2.RandInit();

    std::cout << "Initial parameters:\n\n";
    std::cout << "M1:\n" << m1 << "\nM2:\n" << m2;
    std::cout << "\n\nResult:\n" << Matrix::biggestMatrix(m1,m2);

    std::cout << "Final parameters:\n";
    std::cout << "M1:\n" << m1 << "\nM2:\n" << m2;
//    std::cout << m2 << "\n" << --m1 << "\n" << ++m2 << "\n";
//
//    Dimension dims = getDimensions(m1);
//    std::cout << "Dimensions in m1:\n";
//    dims.Show();
////    auto **m =(double **) getMatrix(m1);
//
//    std::cout << "\nElements in m1 " << elementsNumber(m1) << "\nElements in m2: " << elementsNumber(m2) << "\n";

    return 0;
}