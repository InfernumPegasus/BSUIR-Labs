#include <iostream>
#include <libudev.h>
#include <filesystem>
#include <fstream>
#include <unistd.h>
#include <map>
#include <sys/mount.h>
#include <thread>

using namespace std;
using namespace filesystem;

const int       DRIVE_DEVICE = 1;
const int       NON_DRIVE_DEVICE = 2;

const auto      MOUNTS_FILE = "/proc/mounts";
const auto      INTERFACE_TYPE = "bNumInterfaces";

map<string, path> port_mount_map;

[[noreturn]] void monitor_devices() {
    auto udev { udev_new() };
    auto monitor { udev_monitor_new_from_netlink(udev, "udev") };
    udev_monitor_filter_add_match_subsystem_devtype(monitor, "usb", "usb_device");
    udev_monitor_enable_receiving(monitor);

    while (true) {
        if (auto dev = udev_monitor_receive_device(monitor); dev) {
            string action { udev_device_get_action(dev) };
            string port { udev_device_get_sysname(dev) };
            string device_path {udev_device_get_syspath(dev) };
            path mount_path;

            int device_type;
            if (action == "add") {
                ifstream {device_path + "/" + INTERFACE_TYPE } >> device_type;

                if (device_type == DRIVE_DEVICE) {
                    sleep(3);
                    ifstream mount_list_file {MOUNTS_FILE };

                    string line, temp;
                    do {
                        line = temp;
                        getline(mount_list_file, temp);
                    } while (mount_list_file);

                    istringstream iss {line };
                    string name;
                    iss >> name >> port_mount_map[port];

                    cout << name << endl;
                    mount_path = port_mount_map[port];
                }
            }

            if (action == "remove") {
                device_type = port_mount_map.count(port) == 1 ? DRIVE_DEVICE : NON_DRIVE_DEVICE;
                mount_path = port_mount_map[port];
                port_mount_map.erase(port);
            }

            if (action == "add" || action == "remove") {
                cout << "Action: " << action << endl
                     << "Port name: " << port << endl
                     << "Device path: " << device_path << endl;
                if (device_type == DRIVE_DEVICE)
                    cout << "Drive mount path: " << mount_path << endl;
                cout << endl;
            }
            udev_device_unref(dev);
        }
    }
}

void unmount(const string& port, int flag) {
    auto res = umount2(port_mount_map[port].c_str(), flag);
    if (res < 0 && errno == EBUSY) {
        cout << "USB drive is busy right now" << endl;
    } else if (res < 0) {
        perror("umount2");
    }
}

int main() {
    thread thread_monitor { &monitor_devices };

    while (true) {
        cout << "Enter option:" << endl
             << "1 - Print map info" << endl
             << "2 - Safe unmount" << endl
             << "4 - Quit" << endl << endl;

        string option;
        cout << "Enter option:" << endl;
        getline(cin, option);

        if (option == "1") {
            cout << "[port | mount]" << endl;
            for (auto &port_mount : port_mount_map) {
                cout << port_mount.first << " | " << port_mount.second.string() << endl;
            }
        }

        if (option == "2") {
            string port;
            cout << "Enter port to unmount:" << endl;
            getline(cin, port);
            unmount(port, MNT_EXPIRE);
        }

        if (option == "4") {
            cout << "Quit" << endl;
            exit(0);
        }
    }
}