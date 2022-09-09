#include <iostream>
#include <string>
#include <fstream>
#include <csignal>
#include <cstring>

using namespace std;

static bool show_full_info = false;


string get_parameter_value_str(const string& str, const string& delimiter = "=") {
    return str.substr(str.find(delimiter) + delimiter.length(), str.length());
}

[[maybe_unused]] float get_parameter_value(const string& str, const string& delimiter = "=") {
    return stof(get_parameter_value_str(str, delimiter));
}

void print_remaining_time() {
    system("acpi --battery");
}

void print_power_mode_info() {
    string path = "/sys/firmware/acpi/platform_profile";
    string line;

    ifstream ifs(path);
    if (!ifs.is_open()) {
        cout << "Cannot find 'platform_profile' file!" << endl;
        exit(1);
    }

    getline(ifs, line);
    cout << "Current power save mode: " << line << endl;
}

void print_battery_info() {
    string path = "/sys/class/power_supply/BAT0/uevent";
    string line;

    ifstream ifs(path);
    if (!ifs.is_open()) {
        cout << "Cannot find 'uevent' file!" << endl;
        exit(1);
    }

    while (getline(ifs, line)) {

        if (!show_full_info) {

            if (strstr(line.c_str(), "POWER_SUPPLY_STATUS") != nullptr) {
                cout << "Current battery status: " << get_parameter_value_str(line) << endl;
            }

            if (strstr(line.c_str(), "POWER_SUPPLY_TECHNOLOGY") != nullptr) {
                cout << "Battery technology: " << get_parameter_value_str(line) << endl;
            }

            if (strstr(line.c_str(), "POWER_SUPPLY_CAPACITY_LEVEL") != nullptr) {
                cout << "Capacity level: " << get_parameter_value_str(line) << endl;
            }
        } else {
            cout << line << endl;
        }
    }
    ifs.close();
}

[[noreturn]] void print_full_info() {
    while (true) {
        system("clear");
        cout << "Battery details:" << endl << endl;
        print_remaining_time();
        print_battery_info();
        print_power_mode_info();
        sleep(3);
    }
}

int main(int argc, char **argv) {

    if (argc == 2 && strcmp(argv[1], "-f") == 0) {
        show_full_info = true;
        print_full_info();
    } else if (argc == 2 && strcmp(argv[1], "-s") == 0) {
        system("systemctl suspend");
    } else if (argc == 2 && strcmp(argv[1], "-h") == 0) {
        system("systemctl hibernate");
    } else {
        print_full_info();
    }

}
