#ifndef LIB_USERINPUT_H
#define LIB_USERINPUT_H

class Output
{
public:
    static void printCString(const char *str)
    {
        rewind(stdin);
        for (int i = 0; i < std::strlen(str); ++i)
            std::cout << str[i];
    }
    static std::string printBool(const bool& value)
    {
        std::string buf;
        value != 0 ? buf = std::move("True") : buf = std::move("False");
        return buf;
    }
};

#endif //LIB_USERINPUT_H
