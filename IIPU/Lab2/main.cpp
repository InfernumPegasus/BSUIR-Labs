#include <iostream>
#include <string>
#include <fstream>

using namespace std;

void print_hdd_stats(const string &path) {

    string sda_uevent_path = path + "/uevent";
    string device_wwid_path = path + "/device/wwid";

    string line;

    ifstream sda_uevent_file(sda_uevent_path);
    if (!sda_uevent_file.is_open()) {
        exit(1);
    }

    cout << "'/sys/block/sda/uevent' details:" << endl << endl;
    while (getline(sda_uevent_file, line)) {
        cout << line << endl;
    }
    cout << endl;
    sda_uevent_file.close();

    ifstream device_wwid_file(device_wwid_path);
    if (!device_wwid_file.is_open()) {
        exit(1);
    }

    cout << "'/sys/block/sda/device/wwid' details:" << endl << endl;
    while (getline(device_wwid_file, line)) {
        cout << line << endl;
    }
    device_wwid_file.close();

}

int main() {

    string path1 = "/sys/block/sda";
    print_hdd_stats(path1);

    string path2 = "/sys/block/sdb";
    print_hdd_stats(path2);

    return 0;
}