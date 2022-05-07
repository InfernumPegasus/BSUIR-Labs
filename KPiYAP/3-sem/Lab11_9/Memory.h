#ifndef LAB11_9_MEMORY_H
#define LAB11_9_MEMORY_H

#include <iostream>

namespace PC
{
    class Memory
    {
    protected:
        int m_frequency;
        int m_capacity;
    public:
        explicit Memory(int frequency = 0, int capacity = 0)
                : m_frequency(frequency), m_capacity(capacity)
        {}

        virtual ~Memory();

        virtual void Show() const;
    };
}


#endif //LAB11_9_MEMORY_H
