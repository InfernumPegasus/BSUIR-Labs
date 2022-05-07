//
// Created by USER on 12.09.2021.
//

#include "Worker.h"

void edit(Worker& worker)
{
    std::cout << "Enter Custom ID:\n";
    worker.id = Input::getInt();
    std::cout << "Enter Custom Name:\n";
    worker.m_name = Input::getString();
    std::cout << "Enter Custom Salary:\n";
    worker.m_salary = Input::getDouble();
}

void show(const Worker& worker)
{
    std::cout << "Info:   " << std::endl;
    std::cout << "ID:     " << worker.id << std::endl;
    std::cout << "Name:   " << worker.m_name << std::endl;
    std::cout << "Salary: " << worker.m_salary << std::endl;
    std::cout << std::endl;
}