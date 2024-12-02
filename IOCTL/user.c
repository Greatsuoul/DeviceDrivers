#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdlib.h>

#define DEVICE_NAME "/dev/Driven_class"  // The device file created by the kernel module

// IOCTL command definitions (same as in the kernel module)
#define serial_reset _IO('k', 0)
#define serial_baud_set _IOW('k', 1, int)
#define serial_status _IOR('k', 2, char)
#define serial_control _IOWR('k', 3, char)

int main() {
    int fd;
    int baud_rate = 9600;
    char status;
    char control = 'A';

    // Open the device file created by the kernel module
    fd = open(DEVICE_NAME, O_RDWR);
    if (fd < 0) {
        perror("Failed to open the device");
        return -1;
    }

    // Send a reset command to the device
    if (ioctl(fd, serial_reset) < 0) {
        perror("Failed to send serial_reset command");
        close(fd);
        return -1;
    }
    printf("Serial reset command sent successfully.\n");

    // Set the baud rate using ioctl
    if (ioctl(fd, serial_baud_set, &baud_rate) < 0) {
        perror("Failed to set baud rate");
        close(fd);
        return -1;
    }
    printf("Baud rate set to %d\n", baud_rate);

    // Get the status of the device
    if (ioctl(fd, serial_status, &status) < 0) {
        perror("Failed to get device status");
        close(fd);
        return -1;
    }
    printf("Device status: %c\n", status);

    // Set the control value
    if (ioctl(fd, serial_control, &control) < 0) {
        perror("Failed to set control value");
        close(fd);
        return -1;
    }
    printf("Control set to: %c\n", control);

    // Close the device file
    close(fd);

    return 0;
}
