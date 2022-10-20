#include <cstdio>
#include <sys/io.h>
#include <string>
#include <fstream>
#include <algorithm>

using std::string;

using DWORD = unsigned long;
using WORD  = unsigned int;

#define ROOT_PRIVILEGES_LEVEL       3
#define DEFAULT_PRIVILEGES_LEVEL    0

#define PCI_BASE_ADDR               0x80000000L
#define PCI_MAX_BUS                 255
#define PCI_MAX_DEV                 31
#define PCI_MAX_FUN                 7

/*
 * There are two addresses which we need to use to get access to any pci device.
 *
 * CONFIG_ADDR specifies the configuration address that is required to be accesses.
 * CONFIG_DATA grants access to generate the configuration access.
 */
#define CONFIG_ADDR                 0xCF8
#define CONFIG_DATA                 0xCFC

string device_class_names[] {
        "Unclassified",
        "Mass Storage Controller",
        "Network Controller",
        "Display Controller",
        "Multimedia Controller",
        "Memory Controller",
        "Bridge",
        "Simple Communication Controller",
        "Base System Peripheral",
        "Input Device Controller",
        "Docking Station",
        "Processor",
        "Serial Bus Controller",
        "Wireless Controller",
        "Intelligent Controller",
        "Satellite Communication Controller",
        "Encryption Controller",
        "Signal Processing Controller",
        "Processing Accelerator",
        "Non-Essential Instrumentation",
        "0x3F - Reserved",
        "Co-Processor",
        "0xFE - Reserved",
        "Unassigned Class (Vendor specific)"
};

string string_to_lower(string data) {
    std::transform(data.begin(), data.end(), data.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    return string { data };
}

string convert_to_hex(DWORD num) {
    char hex_string[4];
    sprintf(hex_string, "%lX", num);
    return string { hex_string };
}

void set_privileges_level(int level) {
    if (iopl(level) < 0) {
        string err_msg = "IO privileges setting error";
        if (level == ROOT_PRIVILEGES_LEVEL) {
            err_msg.append(". Execute with root privileges");
        }
        perror(err_msg.c_str());
        exit(1);
    }
    printf("Privileges level set to '%s' level.\n",
           level >= ROOT_PRIVILEGES_LEVEL ? "Root" : "Non-root");
}

[[maybe_unused]] string get_device_class_name(WORD bus, WORD dev, WORD fun) {
    const WORD offset = 0x8;

    DWORD addr = PCI_BASE_ADDR | (bus << 16) | (dev << 11) | (fun << 8) | offset;
    outl(addr, CONFIG_ADDR);
    DWORD data = inl(CONFIG_DATA);

    return device_class_names[data >> 24];
}

string find_device_name(DWORD vendor, DWORD device) {
    std::ifstream ifs("/usr/share/hwdata/pci.ids");

    if (!ifs.is_open()) {
        perror("Cannot open file!");
        exit(1);
    }

    string line;
    string vendor_hex = string_to_lower(convert_to_hex(vendor));
    string device_hex = string_to_lower(convert_to_hex(device));

    string vendor_name;
    string device_name;

    while (getline(ifs, line)) {
        if (line.empty())
            continue;

        if (line.at(0) != '\t' && line.substr(0, 4) == vendor_hex) {
            vendor_name = line.substr(6, line.length() - 6);
            break;
        }
    }

    while (getline(ifs, line)) {
        if (line.empty() || line.at(0) != '\t')
            break;

        if (line.at(0) == '\t' && line.at(1) != '\t' && line.substr(1, 4) == device_hex) {
            device_name = line.substr(7, line.length() - 7);
            break;
        }
    }

    return vendor_name.append(" ").append(device_name);
}

void print_pci_devices_info() {
    for (WORD bus = 0; bus <= PCI_MAX_BUS; bus++) {
        for (WORD dev = 0; dev <= PCI_MAX_DEV; dev++) {
            for (WORD fun = 0; fun <= PCI_MAX_FUN; fun++) {
                /*
                 * TO get started, we should get access to CONFIG_ADDRESS register.
                 * So that we stuff bus, device and function by 16, 11 and 8 (see CONFIG_ADDRESS topology).
                 */
                DWORD addr = PCI_BASE_ADDR | (bus << 16) | (dev << 11) | (fun << 8);

                // Write and the read device address
                outl(addr, CONFIG_ADDR);
                DWORD data = inl(CONFIG_DATA);

                // 0xFFFF - unknown device
                if (((data & 0xFFFF) != 0xFFFF) && (data != 0)) {
                    /*
                     * In PCI device structure,
                     * register 0x0 has two fields: Vendor ID and Device ID.
                     * Both fields have 16-bit size, or 2 bytes.
                     * The lesser address is Vendor ID's address,
                     * then goes Device ID's address
                     */
                    DWORD vendor_id = (data & 0xFFFF);
                    DWORD device_id = (data & 0xFFFF0000) >> 16;

                    string device_name = find_device_name(vendor_id, device_id);
//                    string device_class_name = get_device_class_name(bus, dev, fun);

                    printf("%d:%d:%d Device name: %s\n"
//                           "Device class: %s\n"
                           "\tVendorId: %04lx\n"
                           "\tDeviceId: %04lx\n\n",
                           bus, dev, fun,
                           device_name.c_str(),
//                           device_class_name.c_str(),
                           vendor_id, device_id);
                }
            }
        }
    }
}

int main() {
    set_privileges_level(ROOT_PRIVILEGES_LEVEL);
    print_pci_devices_info();
    set_privileges_level(DEFAULT_PRIVILEGES_LEVEL);

    return 0;
}
