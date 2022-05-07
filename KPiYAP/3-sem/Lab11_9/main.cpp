#include "Computer.h"

#include "../lib/UserInput.h"

int main()
{
    using namespace PC;
    using namespace std;
//    auto *processor = new PC::Processor(10, 20);
//    auto *program   = new PC::Program(true);
//    auto *memory    = new PC::Memory(10000, 6);
//    auto *ram       = new PC::RAM(5000, 20, 4, 10);
//    auto *hdd       = new PC::HDD(4000, 2, 7000, 5);
//
//    auto *computer  = new PC::Computer(processor, memory, ram, hdd, program, 999999);
//    computer->Show();
//
//    delete processor;
//    delete program;
//    delete memory;
//    delete ram;
//    delete hdd;
//    delete computer;

    auto a = 122.32, b = 23.322;

    auto res1 = std::min(a, b);
    auto res2 = PC::min(a, b);
    std::cout << "std::min() result : " << res1 << "\nPC::min()  result : " << res2;

    res1 = std::max(a, b);
    res2 = PC::max(a, b);
    std::cout << "\n\nstd::max() result : " << res1 << "\nPC::max()  result : " << res2;

    return 0;
}
