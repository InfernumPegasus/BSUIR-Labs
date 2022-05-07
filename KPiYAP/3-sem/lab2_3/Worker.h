//
// Created by USER on 12.09.2021.
//

#ifndef LAB2_3_WORKER_H
#define LAB2_3_WORKER_H

#include "D:\Development\C++\Projects\Labs\lib\Input.h"

class Worker
{
private:
    int id;
    std::string m_name;
    double m_salary;

    static int generateID()
    {
        static int s_id = 0;
        return ++s_id;
    }
public:
    friend class Accountant;
    friend void show(const Worker& worker);
    friend void edit(Worker& worker);

    double getSalary() const
    { return m_salary; }

    // конструктор по умолчанию
    explicit Worker(std::string name = "Noname", double salary = 0.0)
    {
        id = generateID();
        m_name = std::move(name);
        m_salary = salary;
    }

    // деструктор
    ~Worker()
    {
        std::cout << "Destructor in Worker" << std::endl;
    }
};

class Accountant
{
public:
    static void setSalary(Worker &Worker, const double& salary)
    {
        Worker.m_salary = salary;
    }
};

#endif //LAB2_3_WORKER_H
