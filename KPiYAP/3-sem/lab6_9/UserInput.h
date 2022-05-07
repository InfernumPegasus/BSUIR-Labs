#ifndef LIB_USERINPUT_H
#define LIB_USERINPUT_H

#include <iostream>
#include <cstring>
#include <typeinfo>

class Input
{
public:
    template<typename T>
    static void getValue (T& _value)
    {
        bool isGood = false;
        do {
            try {
                rewind(stdin);
                scan<T>(_value);
                std::cout << "Your value: " << _value << std::endl;
            }
            catch (std::out_of_range& out_of_range) {
                std::cout << "Out_of_range exception!\n";
                continue;
            }
            catch (std::range_error& range_error) {
                std::cout << "Range_error exception!\n";
                continue;
            }
            catch (std::bad_typeid& bad_typeid) {
                std::cout << "Bad_typeid exception!\n";
                continue;
            }
            catch (std::bad_exception& bad_exception) {
                std::cout << "Bad_exception exception!\n";
                continue;
            }
            catch (std::invalid_argument& invalid_argument) {
                std::cout << "Invalid_argument exception!\n";
                continue;
            }
            isGood = true;
        } while (isGood == false);
    }

    static std::string getStringNoDigs()
    {
        std::string str;

        do {
            rewind(stdin);
            std::cout << "Enter string:" << std::endl;
            std::getline(std::cin, str);
        } while (str.empty() || hasDigits(str));

        return str;
    }
    static char* getStringNoDigs(size_t length)
    {
        char *str = new char[length];
        do {
            rewind(stdin);
            std::cout << "Enter string:" << std::endl;
            char c;
            for (int i = 0; i < length; i++)
            {
                c = std::cin.get();

                if (c == '\n')
                {
                    str[i] = '\0';
                    break;
                }

                str[i] = c;
            }
        } while (str[0] == '\0' || hasDigits(str));
        return str;
    }
    static std::string getString()
    {
        std::string str;

        do {
            rewind(stdin);
            std::cout << "Enter string:" << std::endl;
            std::getline(std::cin, str);
        } while (str.empty());

        return str;
    }
    static char *getString(size_t length)
    {
        char *str = new char[length];
        do {
            rewind(stdin);
            std::cout << "Enter string:" << std::endl;
            char c;
            for (int i = 0; i < length; i++)
            {
                c = std::cin.get();

                if (c == '\n')
                {
                    str[i] = '\0';
                    break;
                }

                str[i] = c;
            }
        } while (str[0] == '\0');
        return str;
    }
    static bool getBoolean()
    {
        int b;
        std::cout << "Enter '1' for True or any value for False boolean value:\n";
        getValue(b);
        return b != 1;
    }
private:
    static bool hasAlpha(const std::string& str)
    {
        for (char i : str)
            if ( isalpha(i) )
                return true;
        return false;
    }
    static bool hasDigits(const std::string &str)
    {
        for (char i : str)
            if (std::isdigit(i))
                return true;
        return false;
    }
    static bool isInRangeASCII(const char c, size_t index1, size_t index2)
    {
        if (index1 < 0 || index2 > 255)
            return false;

        int code = (int )c;
        return ( code >= index1 && code <= index2 );
    }

    template<typename T>
    static void scan (T& _value)
    {
        T value;
        std::string _identifier = typeid(T).name();
        std::string toParse;

        // ввод значения в строку
        std::getline(std::cin, toParse);

        short dots    = 0;
        short minuses = 0;

        // если строка содежрит не-цифры
        for (char i : toParse)
        {
            if (i == '-')
                minuses++;
            if (i == '.')
                dots++;

            if (isalpha(i) || dots > 1 || minuses > 1)
                throw std::invalid_argument("Invalid_argument exception!\n");
        }

        if (_identifier == "i")
            value = std::stoi(toParse);
        if (_identifier == "d")
            value = std::stod(toParse);
        if (_identifier == "l")
            value = std::stol(toParse);
        if (_identifier == "x")
            value = std::stoll(toParse);
        if (_identifier == "e")
            value = std::stold(toParse);
        if (_identifier == "s")
        {
            int temp = std::stoi(toParse);
            if (temp > UINT8_MAX)
                throw std::invalid_argument("Invalid_argument exception!\n");
            value = static_cast<short>(std::stoi(toParse));
        }

        // unsigned values
        if (_identifier == "t")
        {
            if (minuses >= 1)
                throw std::invalid_argument("Invalid_argument exception!\n");
            value = static_cast<unsigned short>(std::stoi(toParse));
        }

        if (_identifier == "j")
        {
            if (minuses >= 1)
                throw std::invalid_argument("Invalid_argument exception!\n");
            value = static_cast<unsigned int>(std::stoi(toParse));
        }

        if (_identifier == "m")
        {
            if (minuses >= 1)
                throw std::invalid_argument("Invalid_argument exception!\n");
            value = std::stoul(toParse);
        }

        if (_identifier == "y")
        {
            if (minuses >= 1)
                throw std::invalid_argument("Invalid_argument exception!\n");
            value = std::stoull(toParse);
        }

        _value = value;
    }
};



#endif //LIB_USERINPUT_H
