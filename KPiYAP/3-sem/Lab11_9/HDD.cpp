#include "HDD.h"

PC::HDD::~HDD()
{
    std::cout << "Destructor of HDD!" << std::endl;
}

void PC::HDD::Show() const
{
    std::cout << "HDD Info:\n"
                 "Frequency : " << m_frequency << std::endl <<
                 "Capacity : " << m_capacity << std::endl <<
                 "RPM : " << m_RPM << std::endl <<
                 "Buffer memory : " << m_bufMem << std::endl << std::endl;
}
