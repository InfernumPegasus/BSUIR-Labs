#include <iostream>
#include <dirent.h>
#include <string>
#include <fstream>
#include <cstring>

using namespace std;

string get_token_by_delimiter(const string& s, const string& delimiter) {
    return s.substr(0, s.find(delimiter));
}

string get_parameter_value_str(const string& str, const string& delimiter = "=") {
    return str.substr(str.find(delimiter) + delimiter.length(), str.length());
}

[[maybe_unused]] float get_parameter_value(const string& str, const string& delimiter = "=") {
    return stof(get_parameter_value_str(str, delimiter));
}

void print_pci_vendor() {

    auto path = "/sys/bus/pci/devices"s;

    DIR *dir;
    struct dirent *entry;

    dir = opendir(path.c_str());

    while ((entry = readdir(dir))) {

        auto dirName = entry->d_name;

        if (dirName[0] != '.') {
            cout << "Device: " << entry->d_name << endl;
            auto uevent_path = path + "/" + dirName + "/uevent";

            string line;

            ifstream ifs(uevent_path);
            if (!ifs.is_open()) {
                cout << "Cannot find 'uevent' file!" << endl;
                exit(1);
            }

            while (getline(ifs, line)) {

                if (strstr(line.c_str(), "PCI_ID") != nullptr) {
                    auto withoutToken = get_parameter_value_str(line, "=");
                    auto vendorId = get_token_by_delimiter(withoutToken, ":");
                    auto deviceId = get_parameter_value_str(withoutToken, ":");

                    printf("VendorId: %s\n"
                           "DeviceId: %s\n",
                           vendorId.c_str(), deviceId.c_str());

                    cout << "Searching for suitable device by [vendor:device] format:" << endl;
                    auto command = "lspci -d "s.append(vendorId).append(":").append(deviceId);
                    system(command.c_str());
                }
            }
            cout << endl;
        }
    }
}

int main() {
    print_pci_vendor();

    return 0;
}

