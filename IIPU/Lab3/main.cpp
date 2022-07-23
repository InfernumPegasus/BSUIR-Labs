#include <iostream>
#include <string>
#include <fstream>
#include <csignal>
#include <cstring>

using namespace std;

static bool show_full_info = false;

void print_battery_info() {

    string path = "/sys/class/power_supply/BAT0/uevent";

    system("clear");
    string line;
    while (true) {

        ifstream ifs(path);
        if (!ifs.is_open()) {
            exit(1);
        }

        cout << "Battery details:" << endl << endl;
        while (getline(ifs, line)) {

            if (!show_full_info) {
                if (strstr(line.c_str(), "POWER_SUPPLY_STATUS") != nullptr ||
                    strstr(line.c_str(), "POWER_SUPPLY_CAPACITY") != nullptr ||
                    strstr(line.c_str(), "POWER_SUPPLY_CAPACITY_LEVEL") != nullptr) {

                    cout << line << endl;
                }
            } else {
                cout << line << endl;
            }

        }

        ifs.close();
        sleep(3);
        system("clear");
    }

}

int main(int argc, char **argv) {

    if (argc == 2 && strcmp(argv[1], "-f") == 0) {
        show_full_info = true;
    }

    print_battery_info();

    return 0;
}
