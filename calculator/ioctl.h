#ifndef IOCTL_H
#define IOCTL_H

#include <linux/ioctl.h>

#define CALC_IOCTL_MAGIC 'C'

#define CMD_ADD _IOWR(CALC_IOCTL_MAGIC, 1, int[2])
#define CMD_SUB _IOWR(CALC_IOCTL_MAGIC, 2, int[2])
#define CMD_MUL _IOWR(CALC_IOCTL_MAGIC, 3, int[2])
#define CMD_DIV _IOWR(CALC_IOCTL_MAGIC, 4, int[2])

#endif // IOCTL_H
