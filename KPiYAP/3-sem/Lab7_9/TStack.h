#ifndef LAB7_9_TSTACK_H
#define LAB7_9_TSTACK_H

#include "../lib/UserOutput.h"
#include "../lib/UserInput.h"

template<typename T, size_t maxSize>
class TStack
{
private:
    size_t m_size = 0;
public:
    T array[maxSize];
public:
    TStack()
    {
        for (size_t i = 0; i < maxSize; i++)
            array[i] = {};
    }

    TStack(TStack<T, maxSize> const& value)
    {
        for (m_size = 0; m_size < value.m_size; m_size++)
        {
            array[m_size] = value.array[m_size];
        }
        for (size_t i = m_size; i < maxSize; i++)
            array[i] = {};
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

    // ПЕРЕГРУЗКА ОПЕРАТОРОВ

    T& operator[] (int index);
    bool operator== (const TStack& value) const;

    TStack& operator+(TStack &value)
    {
        if (m_size == value.m_size)
        {
            for (unsigned int i = 0; i < m_size; i++)
                array[i] += value.array[i];
        }
        return *this;
    }

    friend std::ostream& operator<<(std::ostream& out, const TStack& value)
    {
        if (value.m_size == 0)
            out << "Stack is Empty!\n" << std::endl;
        else {
            out << "Showing stack:" << std::endl;
            for (unsigned int i = 0; i < value.m_size; i++)
                out << "S[" << i << "] = " << value.array[i] << std::endl;
            out << std::endl;
        }
        return out;
    }

    friend std::istream& operator>>(std::istream& in, TStack& value)
    {
        std::cout << "Fulfill stack!" << std::endl;
        for (int i = 0; i < value.m_size; i++)
        {
            std::cout << "Enter value:" << std::endl;
            Input::getValue(value.array[i]);
            // std::cin >> value.array[i];
        }
        return in;
    }

};

#include "TStack.inl"

#endif //LAB7_9_TSTACK_H