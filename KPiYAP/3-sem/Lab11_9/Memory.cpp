#include "Memory.h"

PC::Memory::~Memory()
{
    std::cout << "Destructor of Memory!" << std::endl;
}

void PC::Memory::Show() const
{
    std::cout << "Memory Info:\n"
                 "Frequency : " << m_frequency << std::endl <<
                 "Capacity : " << m_capacity << std::endl << std::endl;
}
