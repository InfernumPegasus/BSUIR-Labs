#include "Program.h"

PC::Program::~Program()
{
    std::cout << "Destructor of RAM!" << std::endl;
}

void PC::Program::Show() const
{
    std::cout << "Program Info:\n"
                 "Is multifunctional : " << std::boolalpha << m_isMultifunctional << std::endl << std::endl;
}