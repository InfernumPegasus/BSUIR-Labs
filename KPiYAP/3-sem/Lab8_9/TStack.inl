#include "TStack.h"

template<typename T, size_t maxSize>
void TStack<T, maxSize>::push(const T &value)
{
    if (!isFull())
    {
        m_array[m_size] = value;
        m_size++;
    } else
        throw StackException("Stack is Full!");
}

template<typename T, size_t maxSize>
void TStack<T, maxSize>::pop()
{
    if (!isEmpty())
        m_size -= 1;
    else
        throw StackException("Stack is Empty!");
}

template<typename T, size_t maxSize>
void TStack<T, maxSize>::print()
{
    if (isEmpty())
        throw StackException("Stack is Empty!");

    std::cout << "Showing Stack:" << std::endl;
    for (size_t i = 0; i < m_size; i++)
        std::cout << m_array[i] << std::endl;
    std::cout << std::endl;
}

template<typename T, size_t maxSize>
int TStack<T, maxSize>::find_first(const T &toFind) const
{
    if (isEmpty())
        throw StackException("Stack is Empty!");
    for (int i = 0; i < m_size; i++)
        if (m_array[i] == toFind)
            return i;
    return -1;
}


template<typename T, size_t maxSize>
void TStack<T, maxSize>::erase()
{
    if (isEmpty())
        throw StackException("Stack is Empty!");
    for (size_t i = 0; i < m_size; i++)
        m_array[i] = {};
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
 * Сортировка
 */

template<typename T, size_t maxSize>
int TStack<T, maxSize>::inc_compare(const void *a, const void *b)
{
    auto v1 = *(T*)a;
    auto v2 = *(T*)b;
    if (v1 < v2)
        return -1;
    if (v1 == v2)
        return 0;
    return 1;
}

template<typename T, size_t maxSize>
int TStack<T, maxSize>::dec_compare(const void *a, const void *b)
{
    auto v1 = *(T*)a;
    auto v2 = *(T*)b;
    if (v1 < v2)
        return 1;
    if (v1 == v2)
        return 0;
    return -1;
}

template<typename T, size_t maxSize>
void TStack<T, maxSize>::sort(bool isInIncOrder)
{
    if (isEmpty())
        throw StackException("Stack is Empty!");

    if (isInIncOrder)
        std::qsort(m_array, m_size, sizeof(m_array[0]), inc_compare);
    else
        std::qsort(m_array, m_size, sizeof(m_array[0]), dec_compare);
}

/*
 * Удаление дубликатов
 */

template<typename T, size_t maxSize>
void TStack<T, maxSize>::removeCopies()
{
    std::vector<T> vector;
    for (size_t i = 0; i < m_size; i++)
        vector.push_back(m_array[i]);

    for (int i = 0; i < vector.size(); i++)
    {
        for (int j = vector.size() - 1; j > i; j--)
            if (vector.at(i) == vector.at(j))
                vector.erase(vector.begin() + j);
    }

    m_size = vector.size();

    m_array = new T[m_size];
    for (size_t i = 0; i < m_size; i++)
        m_array[i] = vector.at(i);
}

/*
 * Вставка элемента по индексу
 */

template<typename T, size_t maxSize>
void TStack<T, maxSize>::insert(size_t index, const T &value)
{
    if (index < 0 || index > m_size)
        throw StackException("Out of range!");

    if (m_size < maxSize)
        m_size++;

    T *newArray = new T[m_size];
    size_t i;

    for (i = 0; i < index; i++)
        newArray[i] = m_array[i];

    newArray[index] = value;

    for (i = index + 1; i < m_size + 1; i++)
        newArray[i] = m_array[i - 1];

    delete[] m_array;

    m_array = newArray;
}



/*
 * Перегрузки операторов
 */

template<typename T, size_t maxSize>
T &TStack<T, maxSize>::operator[](int index)
{ return m_array[index]; }

template<typename T, size_t maxSize>
bool TStack<T, maxSize>::operator==(const TStack &value) const
{
    if (m_size != value.m_size)
        throw StackException("Stack' sizes are not equal!");
    for (int i = 0; i < m_size; i++)
        if (m_array[i] != value.array[i])
            return false;
    return true;
}



