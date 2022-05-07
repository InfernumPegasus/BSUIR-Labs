#include "Computer.h"

PC::Computer::~Computer()
{
    std::cout << "Destructor of Computer!\n";
}

void PC::Computer::Show() const
{
    std::cout << "Computer Info:" << std::endl;
    m_processor->Show();
    m_memory->Show();
    m_ram->Show();
    m_hdd->Show();
    m_program->Show();
    std::cout << "Cost : " << m_cost << std::endl << std::endl;
}


