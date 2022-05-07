#ifndef LAB7_9_TSTACK_H
#define LAB7_9_TSTACK_H

#include <utility>

#include "../lib/UserOutput.h"
#include "../lib/UserInput.h"

class StackException : public std::exception
{
private:
    std::string errorMessage;
public:
    explicit StackException(std::string err_msg) : errorMessage(std::move(err_msg)) {}

    const char *what()
    { return errorMessage.c_str(); }
};

template<typename T, size_t maxSize>
class TStack
{
private:
    size_t m_size = 0;
public:
    T *m_array;
public:
    TStack()
    {
        m_array = new T[maxSize];
    }

    void push(const T& value);
    void pop();
    void print();

    int find_first(const T& toFind) const;
    void erase();

    bool isEmpty() const;
    bool isFull() const;

    size_t size();
    size_t max_size();

    // ДОПЫ

    /*
     * Сортировка
     */

    static int inc_compare(const void *a, const void *b);

    static int dec_compare(const void *a, const void *b);

    void sort(bool isInIncOrder);

    /*
     * Удаление дубликатов
     */

    void removeCopies();

    /*
     * Вставка элемента по индексу
     */

    void insert(size_t index, const T &value);

    /*
     * ПЕРЕГРУЗКА ОПЕРАТОРОВ
     */

    T& operator[] (int index);
    bool operator== (const TStack& value) const;

    TStack& operator+ (TStack &value)
    {
        if (m_size == value.m_size)
        {
            for (size_t i = 0; i < m_size; i++)
                m_array[i] += value.array[i];
        }
        return *this;
    }

    friend std::ostream& operator<<(std::ostream& out, const TStack& value)
    {
        if (value.m_size == 0)
            out << "Stack is Empty!\n" << std::endl;
        else {
            out << "Showing stack:" << std::endl;
            for (size_t i = 0; i < value.m_size; i++)
                out << "S[" << i << "] = " << value.array[i] << std::endl;
            out << std::endl;
        }
        return out;
    }
};

#include "TStack.inl"

#endif //LAB7_9_TSTACK_H