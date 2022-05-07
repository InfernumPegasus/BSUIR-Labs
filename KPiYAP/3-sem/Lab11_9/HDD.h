#ifndef LAB11_9_HDD_H
#define LAB11_9_HDD_H

#include <iostream>
#include "Memory.h"

namespace PC
{
    class HDD : protected PC::Memory
    {
    private:
        int m_RPM;
        int m_bufMem;
    public:
        explicit HDD(int frequency = 0, int capacity = 0, int RPM = 0, int bufMem = 0)
                : Memory(frequency, capacity), m_RPM(RPM), m_bufMem(bufMem)
        {}

        ~HDD() override;

        void Show() const final;
    };
}


#endif //LAB11_9_HDD_H
