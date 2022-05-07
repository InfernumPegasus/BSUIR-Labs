#include "Processor.h"

PC::Processor::~Processor()
{
    std::cout << "Destructor of Processor!" << std::endl;
}

void PC::Processor::Show() const
{
    std::cout << "Processor Info:\n"
                 "Frequency : " << m_frequency << std::endl <<
                 "Core amount : " << m_coreAmount << std::endl << std::endl;
}

