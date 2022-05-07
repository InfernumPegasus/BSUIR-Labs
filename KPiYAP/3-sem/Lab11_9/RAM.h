#ifndef LAB11_9_RAM_H
#define LAB11_9_RAM_H

#include <iostream>
#include "Memory.h"

namespace PC
{
    class RAM : protected PC::Memory
    {
    private:
        int m_standard;
        int m_timing;
    public:
        explicit RAM(int frequency = 0, int capacity = 0, int standard = 0, int timing = 0)
                : Memory(frequency, capacity), m_standard(standard), m_timing(timing)
        {}

        ~RAM() override;

        void Show() const final;
    };
}

#endif //LAB11_9_RAM_H
