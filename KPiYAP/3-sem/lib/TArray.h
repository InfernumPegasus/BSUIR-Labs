#ifndef TEST_TARRAY_H
#define TEST_TARRAY_H

template<class T>
class Array
{
private:
    T *m_array;
    int m_length;
public:
    // Default Constructor
    Array()
    {
        m_length = 0;
        m_array = nullptr;
    }

    // Main constructor
    explicit Array(int len)
    {
        m_length = len;
        m_array = new T[m_length];
    }

    ~Array()
    {
        delete [] m_array;
    }

    // Erasing array
    void Erase()
    {
        delete [] m_array;
        m_length = 0;
        m_array = nullptr;
    }

    // Overloaded operator[]
    T& operator[](int index)
    {
        return m_array[index];
    }

    // Function to get array length
    int getLength();
};

template<typename T>
int Array<T>::getLength()
{
    return m_length;
}

#endif //TEST_TARRAY_H
