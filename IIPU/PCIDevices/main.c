#include <stdio.h>
#include <pci/pci.h>

void print_pci_devices_info() {
    struct pci_access *pci_access;
    struct pci_dev *dev;

    char name_buf[1024];
    u16 vendor_id;
    u16 device_id;

    int device_counter = 0;

    // Initialize PCI access
    pci_access = pci_alloc();
    pci_init(pci_access);

    // Scanning of devices
    pci_scan_bus(pci_access);

    for (dev = pci_access->devices; dev; dev = dev->next, device_counter++) {
        // Filling info using flags
        // TODO learn flags
        pci_fill_info(dev, PCI_FILL_IDENT | PCI_FILL_BASES | PCI_FILL_CLASS);
        vendor_id = dev->vendor_id;
        device_id = dev->device_id;

        // Get device name using vendor_id and device_id
        pci_lookup_name(pci_access,
                        name_buf,
                        sizeof(name_buf),
                        PCI_LOOKUP_DEVICE,
                        vendor_id,
                        device_id);

        printf("%s\n", name_buf);
        printf("%u\n", vendor_id);
        printf("%u\n\n", device_id);
    }

    pci_cleanup(pci_access);

    printf("Total devices found: %d\n\n", device_counter);
}

int main() {
    print_pci_devices_info();

    return 0;
}
