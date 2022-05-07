#include <iostream>
#include <vector>

#include "TStack.h"

template<class T>
int compare(const void *a, const void *b)
{
    auto v1 = *(T*)a;
    auto v2 = *(T*)b;
    if (v1 < v2)
        return -1;
    if (v1 == v2)
        return 0;
    return 1;
}

int str_compare (const void *a, const void *b)
{return strcmp(*(char**)a, *(char**)b);}

bool are_strings_equal(char *array[], size_t size)
{
    if (size < 1)
    {
        std::cout << "No info!" << std::endl;
        throw std::bad_exception();
    }

    size_t len = strlen(array[0]);
    for (size_t i = 0; i < size; i++)
        if (strlen(array[i]) != len)
            return false;
    return true;
}

template<class T, unsigned int maxSize>
void sort(TStack<T, maxSize>& stack)
{
    std::cout << "Using template<T> sorting specialization!" << std::endl;
    std::qsort(stack.array, stack.size(), sizeof(T), compare<T>);
}

template<unsigned int maxSize>
void sort(TStack<char *, maxSize>& stack)
{
    std::cout << "Using char* sorting specialization!" << std::endl;

    if (!are_strings_equal(stack.array, stack.size()))
    {
        std::cout << "Strings are not Equal and will not be sorted!" << std::endl;
        return;
    }

    for (int i = 0; i < stack.size(); i++)
        std::qsort(stack.array, stack.size(), sizeof(stack.array[i]), str_compare);
}

/*
 * Допы:
 * 1 - добавление элементов стека с клавиатуры
 * 2 - просмотр стека
 * 3 - редактирование определенного стека по индексу
 * 4 - удаление выбранного стека
 * 5 - сравнение двух стеков по элементам
 * 6 - копирование стеков
 * 7 - удаление дубликатов
 */

const int stack_size = 5;

int menu()
{
    std::cout << "1 - Create Stack\n"
                 "2 - Print Stack\n"
                 "3 - Edit Stack\n"
                 "4 - Delete Stack\n"
                 "5 - Compare Stacks\n"
                 "6 - Copy Stack\n"
                 "7 - Delete copies\n"
                 "8 - Exit\n\n";
    int option;
    do {
        std::cout << "Choose option!\n";
        Input::getValue(option);
    } while (option < 1 || option > 82);

    return option;
}

template <class T>
void create(std::vector<TStack<T, stack_size>> &vector)
{
    vector.emplace(vector.cbegin(), TStack<T, stack_size>());
}

template<class T>
void print(const std::vector<T> &vector)
{
    for (size_t i = 0; i < vector.size(); i++)
        std::cout << "Stack " << i << " :" << std::endl << vector.at(i);
}

template<class T>
void edit(std::vector<T> &vector)
{
    if (vector.empty())
    {
        std::cout << "No info!" << std::endl;
        return;
    }

    size_t index;
    do {
        std::cout << "Enter index from " << 0 << " to " << vector.size() - 1 << std::endl;
//        std::cin >> index;
        Input::getValue(index);
    } while (index < 0 || index >= vector.size());

    std::cin >> vector.at(index);
}

template<class T>
void delete_by_index(std::vector<T> &vector)
{
    if (vector.empty())
    {
        std::cout << "No info!" << std::endl;
        return;
    }

    size_t index;
    do {
        std::cout << "Enter index from " << 0 << " to " << vector.size() - 1 << std::endl;
//        std::cin >> index;
        Input::getValue(index);
    } while (index < 0 || index >= vector.size());

    vector.erase(vector.begin() + index);
}

template<class T>
void compare(T value1, T value2)
{
//    bool res = value1 == value2;
    if (value1 == value2)
        std::cout << "Equal!" << std::endl;
    else
        std::cout << "Not Equal!" << std::endl;
}

template<class T>
void compare_menu(const std::vector<T> &vector)
{
    size_t index1;
    size_t index2;

    do {
        std::cout << "Enter index of the first stack!" << std::endl;
        std::cout << "Enter index from " << 0 << " to " << vector.size() - 1 << std::endl;
//        std::cin >> index1;
        Input::getValue(index1);
    } while (index1 < 0 || index1 >= vector.size());

    do {
        std::cout << "Enter index of the second stack!" << std::endl;
        std::cout << "Enter index from " << 0 << " to " << vector.size() - 1 << std::endl;
//        std::cin >> index2;
        Input::getValue(index2);
    } while (index2 < 0 || index2 >= vector.size());

    compare(vector.at(index1), vector.at(index2));
}

template<class T>
void copy(const T& source, T& dest)
{
//    source(dest);
    dest = source;
}

template<class T>
void copy_menu(std::vector<T> &vector)
{
    size_t index1;
    size_t index2;

    do {
        std::cout << "Enter index of the source stack!" << std::endl;
        std::cout << "Enter index from " << 0 << " to " << vector.size() - 1 << std::endl;
//        std::cin >> index1;
        Input::getValue(index1);
    } while (index1 < 0 || index1 >= vector.size());

    do {
        std::cout << "Enter index of the stack to fill!" << std::endl;
        std::cout << "Enter index from " << 0 << " to " << vector.size() - 1 << std::endl;
//        std::cin >> index2;
        Input::getValue(index2);
    } while (index2 < 0 || index2 >= vector.size());

    copy(vector.at(index1), vector.at(index2));
}

template<class T>
void delete_copies(std::vector<T> &vector)
{
    for (int i = 0; i < vector.size(); i++)
    {
        for (int j = vector.size() - 1; j > i; j--)
        {
            if (vector.at(i) == vector.at(j))
                vector.erase(vector.begin() + j);
        }
    }

}

int main()
{
    std::vector<TStack<int, stack_size>> vector;

    TStack<int, stack_size> stack1;
    stack1.push(3);
    stack1.push(65);
    stack1.push(1);
    stack1.push(111);

    TStack<int, stack_size> stack2;
    stack2.push(3);
    stack2.push(65);
    stack2.push(1);
    stack2.push(111);

    TStack<int, stack_size> stack3;
    stack3.push(56);
    stack3.push(321);
    stack3.push(97);

    TStack<int, stack_size> stack4;
    stack4.push(3);
    stack4.push(65);
    stack4.push(1);
    stack4.push(111);

    vector.push_back(stack1);
    vector.push_back(stack2);
    vector.push_back(stack3);
    vector.push_back(stack4);

    while (true)
    {
        switch (menu())
        {
            case 1:
                create(vector);
                break;
            case 2:
                print(vector);
                break;
            case 3:
                edit(vector);
                break;
            case 4:
                delete_by_index(vector);
                break;
            case 5:
                compare_menu(vector);
                break;
            case 6:
                copy_menu(vector);
                break;
            case 7:
                delete_copies(vector);
                break;
            case 8:
                return 0;
        }
    }

//    TStack<int, 7> stack1;
//    stack1.push(3);
//    stack1.push(22);
//    stack1.push(5);
//    stack1.push(113);
//    stack1.push(43);
//    stack1.push(8675645);
//
//    std::cout << stack1;
//    sort(stack1);
//    std::cout << stack1;
//
//    std::cout << "First " << 113 << " has Index: " << stack1.find_first(113) << std::endl;


    return 0;
}