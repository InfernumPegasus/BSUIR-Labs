#ifndef LAB11_9_PROCESSOR_H
#define LAB11_9_PROCESSOR_H

#include <iostream>

namespace PC
{
    class Processor
    {
    private:
        int m_frequency;
        int m_coreAmount;
    public:
        explicit Processor(int frequency = 0, int coreAmount = 0)
                : m_frequency(frequency), m_coreAmount(coreAmount)
        {}

        ~Processor();

        void Show() const;
    };
}


#endif //LAB11_9_PROCESSOR_H
