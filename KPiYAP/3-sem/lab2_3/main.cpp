#include <vector>

#include "Worker.h"
#include "D:\Development\C++\Projects\Labs\lib\Input.h"

// создание через std::list
// вывод всех
// редактирвание
// вывод сотрудников по меньшей ЗП

void addWorker(std::vector<Worker> &workers)
{
    std::cout << "Enter Custom Name:\n";
    std::string name = Input::getString();

    std::cout << "Enter Salary:\n";
    double salary = Input::getDouble();

    workers.emplace_back(name);

    Accountant::setSalary(workers.back(), salary);
}

int menu()
{
    std::cout << "Choose option.\n";
    std::cout << "1 - Add Worker\n"
                 "2 - Show all Workers\n"
                 "3 - Edit Worker\n"
                 "4 - Show Workers with less Salary\n\n"
                 "5 - Exit\n";
    return Input::getInt();
}

int main()
{
    std::vector<Worker> workers;

    while (true)
    {
        double tempSalary;
        switch (menu())
        {
            case 1:
                addWorker(workers);
                break;
            case 2:
                for (auto& item : workers)
                    show(item);
                break;
            case 3:
                int index;
                do {
                    std::cout << "Enter Index:\n";
                    index = Input::getInt();
                } while (index < 0 || index >= workers.size());

                std::cout << "Do you want to change salary?\n";
                edit(workers[index]);
                break;
            case 4:
                std::cout << "Enter Salary to Search:\n";
                tempSalary = Input::getDouble();

                for (auto & worker : workers)
                    if (tempSalary > worker.getSalary())
                        show(worker);

                break;
            case 5:
                return 0;
        }
    }
}

