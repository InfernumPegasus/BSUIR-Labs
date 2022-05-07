#include "TVlib.h"

int main()
{
//    TV tv(12.7);
//
//    LCD lcd(14.2, "pupik");
//    Plasma plasma(432, true);
//
//    LG lg(234.2, "random tech", "random name");
//    Philips philips(12.2, true);
//
//    std::cout << "\n\n";
//    tv.Info();
//    std::cout << "\n";
//    lcd.Info();
//    std::cout << "\n";
//    plasma.Info();
//    std::cout << "\n";
//    lg.Info();
//    std::cout << "\n";
//    philips.Info();
//    std::cout << "\n\n";

    double diag;
    std::string tech;
    bool hasVR;
    std::string name;
    std::string userName;
    double cost;

    std::cout << "Enter Diagonal!\n";
    Input::getValue(diag);
    std::cout << "Enter Tech!\n";
    tech = Input::getStringNoDigs();
    std::cout << "Has it VR?\n";
    hasVR = Input::getBoolean();
    std::cout << "Enter Name!\n";
    name = Input::getStringNoDigs();
    std::cout << "Enter UserName!\n";
    userName = Input::getStringNoDigs();
    std::cout << "Enter Cost!\n";
    Input::getValue(cost);

    HomeTV homeTv(diag, tech, hasVR, name, userName, cost);
    homeTv.Info();
    std::cout << "\n";

    return 0;
}
