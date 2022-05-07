#include "TStack.h"

template<typename T, size_t maxSize>
void TStack<T, maxSize>::push(const T &value)
{
    if (!isFull())
    {
        array[m_size] = value;
        m_size++;
    } else
        std::cout << "Stack is Full!" << std::endl;
}

template<typename T, size_t maxSize>
void TStack<T, maxSize>::pop()
{
    if (!isEmpty())
        m_size -= 1;
    else
        std::cout << "Stack is Empty!" << std::endl;
}

template<typename T, size_t maxSize>
void TStack<T, maxSize>::print()
{
    if (isEmpty())
    {
        std::cout << "Stack is Empty!" << std::endl;
        return;
    }

    for (auto & item : array)
        std::cout << item << std::endl;
}

template<typename T, size_t maxSize>
int TStack<T, maxSize>::find_first(const T &toFind) const
{
    if (!isEmpty())
        for (int i = 0; i < m_size; i++)
            if (array[i] == toFind)
                return i;
    return T{};
}


template<typename T, size_t maxSize>
void TStack<T, maxSize>::erase()
{
    for (size_t i = 0; i < m_size; i++)
    {
        array[i] = {};
    }
    m_size = 0;
}

template<typename T, size_t maxSize>
bool TStack<T, maxSize>::isEmpty() const
{ return m_size < 1; }

template<typename T, size_t maxSize>
bool TStack<T, maxSize>::isFull() const
{ return m_size == maxSize; }

template<typename T, size_t maxSize>
size_t TStack<T, maxSize>::size()
{ return m_size; }

template<typename T, size_t maxSize>
size_t TStack<T, maxSize>::max_size()
{ return maxSize; }

/*
 * Перегрузки операторов
 */

template<typename T, size_t maxSize>
T &TStack<T, maxSize>::operator[](int index)
{ return array[index]; }

template<typename T, size_t maxSize>
bool TStack<T, maxSize>::operator==(const TStack &value) const
{
    if (m_size != value.m_size)
        return false;
    for (int i = 0; i < m_size; i++)
        if (array[i] != value.array[i])
            return false;
    return true;
}

