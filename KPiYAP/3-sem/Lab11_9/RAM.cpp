#include "RAM.h"

PC::RAM::~RAM()
{
    std::cout << "Destructor of RAM!" << std::endl;
}

void PC::RAM::Show() const
{
    std::cout << "RAM Info:\n"
                 "Frequency : " << m_frequency << std::endl <<
                 "Capacity : " << m_capacity << std::endl <<
                 "Standard : DDR " << m_standard << std::endl <<
                 "Timing : " << m_timing << std::endl << std::endl;
}
