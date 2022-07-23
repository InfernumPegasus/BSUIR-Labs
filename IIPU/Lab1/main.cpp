#include <iostream>
#include <dirent.h>
#include <string>
#include <fstream>

using namespace std;

void print_pci_vendor(const string &path) {

    DIR *dir;
    struct dirent *entry;

    dir = opendir(path.c_str());

    while ((entry = readdir(dir))) {

        const string dirName = entry->d_name;

        if (dirName[0] != '.') {
            cout << "device: " << entry->d_name << endl;
            auto vendor_path = path + "/" + dirName + "/vendor";
            auto device_path = path + "/" + dirName + "/device";

            ifstream vendor_file(vendor_path);
            if (!vendor_file.is_open()) {
                exit(1);
            }

            ifstream device_file(device_path);
            if (!device_file.is_open()) {
                exit(1);
            }

            string vendor_id;
            string device_id;

            getline(vendor_file, vendor_id);
            getline(device_file, device_id);

            cout << "vendor id: " << vendor_id << endl;
            cout << "device id: " << device_id << endl << endl;

            vendor_file.close();
            device_file.close();
        }
    }
}

int main() {

    string path = "/sys/bus/pci/devices";

    print_pci_vendor(path);

    return 0;
}
