#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include "ioctl.h"

int main()
{
    int fd, operands[2], user, result;

    // Open the device
    fd = open("/dev/MyCalcDev", O_RDWR);
    if (fd < 0)
    {
        perror("Failed to open device");
        return -1;
    }

    // Set operands
    operands[0] = 10;
    operands[1] = 20;
    // Perform addition via ioctl
    while (1)
    {
        printf("Enter Choice 1)ADD 2)SUB 3)MUL 4)DIV");
        scanf("%d", &user);
        switch (user)
        {
        case 1:
            ioctl(fd, CMD_ADD, operands);
            result = operands[0];
            printf("Result of addition: %d\n", result);
            break;
        case 2:
            ioctl(fd, CMD_SUB, operands); // Retrieve result from operands array
            result = operands[0];         // ioctl updates this with the result
            printf("Result of subtraction: %d\n", result);
            break;
        case 3:
            ioctl(fd, CMD_MUL, operands); // Retrieve result from operands array
            result = operands[0];         // ioctl updates this with the result
            printf("Result of MUL: %d\n", result);
            break;
        case  4:
            ioctl(fd, CMD_DIV, operands); // Retrieve result from operands array
            result = operands[0];         // ioctl updates this with the result
            printf("Result of DIV: %d\n", result);
            break;
        
        default:
            printf("Enter valid choice");
        break;
        
        }
    }
        close(fd);
        return 0;
    }
