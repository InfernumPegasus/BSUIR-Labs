#include <string>
#include <cstring>
#include <fstream>

using namespace std;

bool use_utility = false;

void print_pci_names() {
    auto path = "/proc/bus/pci/devices"s;
    string line;

    ifstream ifs(path);
    if (!ifs.is_open()) {
        printf("Cannot find 'devices' file!\n");
        exit(1);
    }

    while (getline(ifs, line)) {
        auto device_slot = line.substr(0, 4);
        printf("Device slot: %s\n", device_slot.c_str());

        auto vendor_id = line.substr(device_slot.length() + 1, 4);
        printf("Vendor Id: %s\n", vendor_id.c_str());

        auto device_id = line.substr(device_slot.length() + vendor_id.length() + 1, 4);
        printf("Device Id: %s\n", device_id.c_str());

        ulong last_space_pos = line.find_last_of('0');

        auto device_name = line.substr(last_space_pos + 2, line.length());
        printf("Device name: %s\n", device_name.c_str());

        if (use_utility) {
            printf("Searching for suitable device by [%s:%s] :\n",
                   vendor_id.c_str(), device_id.c_str());
            auto command = "lspci -d "s.append(vendor_id).append(":").append(device_id);
            system(command.c_str());
        }

        printf("\n");
    }
}

int main(int argc, char** argv) {

    if (argc == 2 && strcmp(argv[1], "--util") == 0) {
        use_utility = true;
    }

    print_pci_names();

    return 0;
}

