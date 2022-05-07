#include "BracketsCheck.h"

int main()
{
    std::string source_file = "Brackets.txt";
    std::string result_file = "Result.txt";

    BracketsCheck test(source_file, result_file);
    try {
        test.loadFromTxt();
        test.check();
        test.saveInTxt();
    }
    catch (UserException &exception) {
        std::cout << exception.what() << std::endl << "End of the program!" << std::endl;
    }

    return 0;
}