#ifndef LAB11_9_COMPUTER_H
#define LAB11_9_COMPUTER_H

#include "Processor.h"
#include "Memory.h"
#include "RAM.h"
#include "HDD.h"
#include "Program.h"

namespace PC
{
    template<class T>
    const T& min(const T& a, const T& b)
    {
        if (a > b)
            return b;
        return a;
    }

    template<class T>
    const T& max(const T& a, const T& b)
    {
        if (a > b)
            return a;
        return b;
    }

    class Computer
    {
    protected:
        PC::Processor *m_processor;
        PC::Memory    *m_memory;
        PC::RAM       *m_ram;
        PC::HDD       *m_hdd;
        PC::Program   *m_program;

        int m_cost;
    public:
        explicit Computer(PC::Processor *proc = nullptr, PC::Memory *mem = nullptr,
                PC::RAM *ram = nullptr, PC::HDD *hdd = nullptr, PC::Program *program = nullptr, int cost = 0)
                : m_processor(proc), m_memory(mem), m_ram(ram), m_hdd(hdd), m_program(program), m_cost(cost)
        {}

        ~Computer();

        void Show() const;
    };
}


#endif //LAB11_9_COMPUTER_H
