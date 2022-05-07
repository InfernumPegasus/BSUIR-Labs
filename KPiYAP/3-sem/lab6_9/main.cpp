#include "TVlib.h"

// доп
// диагональ вводится с клавиатуры
// фукнция расчета coeff использует введенную диагональ, технологию, тип матрицы и VR (в зависимости от введенных значений)
// вывести коэффициент стоимости для каждого объекта

int main()
{
    LCD lcd;
//    LCD lcd(16, "greg-tech");
    lcd.calculateCostCoefficient();

    Plasma plasma;
//    Plasma plasma(30, true);
    plasma.calculateCostCoefficient();

    LG lg;
//    LG lg(234.2, "random tech", "AMOLED");
    lg.calculateCostCoefficient();

    TV *array[3];
    array[0] = &lcd;
    array[1] = &plasma;
    array[2] = &lg;

    for (auto item : array)
    {
        item->fill();
        std::cout << std::endl;
    }

    std::cout << std::endl << "Info!" << std::endl;
    for (auto item : array)
    {
        item->Info();
        std::cout << std::endl;
    }

    return 0;
}
