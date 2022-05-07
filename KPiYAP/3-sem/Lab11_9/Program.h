#ifndef LAB11_9_PROGRAM_H
#define LAB11_9_PROGRAM_H

#include <iostream>

namespace PC
{
    class Program
    {
    private:
        bool m_isMultifunctional;
    public:
        explicit Program(bool isMultifunctional = false)
                : m_isMultifunctional(isMultifunctional)
        {}

        ~Program();

        void Show() const;
    };
}

#endif //LAB11_9_PROGRAM_H
