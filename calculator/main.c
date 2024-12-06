#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/ioctl.h>
#include "ioctl.h"

#define mem_size 1024

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Saishiva Reddy");
MODULE_DESCRIPTION("IOCTL Calculator Module");

dev_t dev = 0;
static struct cdev my_dev;

static int __init char_driver_init(void);
static void __exit char_driver_exit(void);

static int my_open(struct inode *inode, struct file *file);
static int my_release(struct inode *inode, struct file *file);
static long my_ioctl(struct file *file, unsigned int cmd, unsigned long arg);

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = my_open,
    .release = my_release,
    .unlocked_ioctl = my_ioctl,
};

static int my_open(struct inode *inode, struct file *file)
{
    pr_info("\nDevice file opened\n");
    return 0;
}

static int my_release(struct inode *inode, struct file *file)
{
    pr_info("\nDevice file closed\n");
    return 0;
}

static long my_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    int operands[2] = {0};
    int result = 0;

    switch (cmd) {
    case CMD_ADD:
        if (copy_from_user(operands, (int __user *)arg, sizeof(operands))) {
            pr_err("\nFailed to copy operands from user space\n");
            return -EFAULT;
        }
        result = operands[0] + operands[1];
        pr_info("\nAddition: %d + %d = %d\n", operands[0], operands[1], result);

        if (copy_to_user((int __user *)arg, &result, sizeof(result))) {
            pr_err("\nFailed to copy result to user space\n");
            return -EFAULT;
        }
        break;

     case CMD_SUB:
        if (copy_from_user(operands, (int __user *)arg, sizeof(operands))) {
            pr_err("\nFailed to copy operands from user space\n");
            return -EFAULT;
        }
        result = operands[0] - operands[1];
        pr_info("\nSUB: %d - %d = %d\n", operands[0], operands[1], result);

        if (copy_to_user((int __user *)arg, &result, sizeof(result))) {
            pr_err("\nFailed to copy result to user space\n");
            return -EFAULT;
        }
        break;

     case CMD_MUL:
        if (copy_from_user(operands, (int __user *)arg, sizeof(operands))) {
            pr_err("\nFailed to copy operands from user space\n");
            return -EFAULT;
        }
        result = operands[0] * operands[1];
        pr_info("\nMUL: %d * %d = %d\n", operands[0], operands[1], result);

        if (copy_to_user((int __user *)arg, &result, sizeof(result))) {
            pr_err("\nFailed to copy result to user space\n");
            return -EFAULT;
        }
        break;
    
     case CMD_DIV:
        if (copy_from_user(operands, (int __user *)arg, sizeof(operands))) {
            pr_err("\nFailed to copy operands from user space\n");
            return -EFAULT;
        }
        result = operands[0] / operands[1];
        pr_info("\nDIV: %d / %d = %d\n", operands[0], operands[1], result);

        if (copy_to_user((int __user *)arg, &result, sizeof(result))) {
            pr_err("\nFailed to copy result to user space\n");
            return -EFAULT;
        }
        break;

    default:
        pr_info("\nInvalid IOCTL command\n");
        return -EINVAL;
    }
    return 0;
}

static int __init char_driver_init(void)
{
    if (alloc_chrdev_region(&dev, 0, 1, "MyCalcDev") < 0) {
        pr_err("\nError allocating Major Number\n");
        return -1;
    }
    pr_info("\nMajor = %d, Minor = %d\n", MAJOR(dev), MINOR(dev));

    cdev_init(&my_dev, &fops);
    if (cdev_add(&my_dev, dev, 1) < 0) {
        pr_err("\nError adding device\n");
        return -1;
    }
    return 0;
}

static void __exit char_driver_exit(void)
{
    cdev_del(&my_dev);
    unregister_chrdev_region(dev, 1);
    pr_info("\nDriver removed successfully\n");
}

module_init(char_driver_init);
module_exit(char_driver_exit);
