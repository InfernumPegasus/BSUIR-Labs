#ifndef LIB_USEROUTPUT_H
#define LIB_USEROUTPUT_H

#include <iostream>
#include <cstring>
#include <string>

class Output
{
public:
    static void printCString(const char *str)
    {
        rewind(stdin);
        for (int i = 0; i < std::strlen(str); ++i)
            std::cout << str[i];
    }

    static void printBool(const bool& value)
    {
        value != 0 ? std::cout << "True" : std::cout << "False";
    }

    static std::string boolToString(const bool value)
    {
        if (value)
            return "True";
        return "False";
    }


};

#endif //LIB_USEROUTPUT_H