#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/err.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/ioctl.h>

#define MEM_SIZE 1024
#define serial_magic 'k'

// Define IOCTL commands
#define serial_reset _IO(serial_magic, 0)
#define serial_baud_set _IOW(serial_magic, 1, int)
#define serial_status _IOR(serial_magic, 2, char)
#define serial_control _IOWR(serial_magic, 3, char)

dev_t dev = 0;
static struct cdev file_cdev;
static char *kernel_buffer;
static struct class *dev_class;

// Hardware simulation addresses (Just for illustration)
#define address_baud_reg 0x00
#define address_status_reg 0x01
#define address_control_reg 0x02

static long FILE_IOCTL(struct file *file, unsigned int cmd, unsigned long arg);
static int FILE_OPEN(struct inode *inode, struct file *file);
static int FILE_RELEASE(struct inode *inode, struct file *file);
static ssize_t FILE_READ(struct file *file, char __user *buff, size_t len, loff_t *off);
static ssize_t FILE_WRITE(struct file *file, const char __user *buff, size_t len, loff_t *off);

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .read = FILE_READ,
    .write = FILE_WRITE,
    .open = FILE_OPEN,
    .release = FILE_RELEASE,
    .unlocked_ioctl = FILE_IOCTL,  // Use unlocked_ioctl instead of ioctl
};

// Open function for the device
static int FILE_OPEN(struct inode *inode, struct file *file) {
    pr_info("driver open successfully\n");
    return 0;
}

// Release function for the device
static int FILE_RELEASE(struct inode *inode, struct file *file) {
    pr_info("driver release successfully\n");
    return 0;
}

// Read function for the device
static ssize_t FILE_READ(struct file *file, char __user *buff, size_t len, loff_t *off) {
    if (copy_to_user(buff, kernel_buffer, MEM_SIZE)) {
        pr_err("Error in data read!!!\n");
        return -1;
    }
    pr_info("driver read successfully\n");
    return MEM_SIZE;
}

// Write function for the device
static ssize_t FILE_WRITE(struct file *file, const char __user *buff, size_t len, loff_t *off) {
    if (copy_from_user(kernel_buffer, buff, len)) {
        pr_err("Error in data write!!!\n");
        return -EFAULT;
    }
    pr_info("driver write successfully\n");
    return len;
}

// IOCTL handler function
static long FILE_IOCTL(struct file *file, unsigned int cmd, unsigned long arg) {
    int retval = 0;
    unsigned int baud_value;
    unsigned char status, control;

    switch (cmd) {
        case serial_reset:
            pr_info("Serial reset command received\n");
            // Implement reset logic here, if needed
            break;

        case serial_baud_set:
            if (copy_from_user(&baud_value, (int __user *)arg, sizeof(int))) {
                pr_err("Failed to copy baud value from user\n");
                return -EFAULT;
            }
            pr_info("Setting baud rate to %d\n", baud_value);
            // Simulate writing the baud value to hardware
            outw(baud_value, address_baud_reg);
            break;

        case serial_status:
            // Simulate reading status from hardware
            status = inb(address_status_reg);
            if (copy_to_user((char __user *)arg, &status, sizeof(char))) {
                pr_err("Failed to copy status to user\n");
                return -EFAULT;
            }
            pr_info("Returning status: %c\n", status);
            break;

        case serial_control:
            if (copy_from_user(&control, (char __user *)arg, sizeof(char))) {
                pr_err("Failed to copy control value from user\n");
                return -EFAULT;
            }
            pr_info("Setting control to %c\n", control);
            // Simulate writing the control value to hardware
            outb(control, address_control_reg);

            // Read the control register after setting
            control = inb(address_control_reg);
            if (copy_to_user((char __user *)arg, &control, sizeof(char))) {
                pr_err("Failed to copy control to user\n");
                return -EFAULT;
            }
            break;

        default:
            pr_err("Invalid IOCTL command\n");
            retval = -EINVAL;
            break;
    }

    return retval;
}

// Initialize the module
static int __init char1_init(void) {
    if (alloc_chrdev_region(&dev, 0, 1, "character_device") < 0) {
        pr_info("Character device allocation failed\n");
        return -1;
    }
    pr_info("Major=%d, Minor=%d\n", MAJOR(dev), MINOR(dev));

    cdev_init(&file_cdev, &fops);

    if (cdev_add(&file_cdev, dev, 1) < 0) {
        unregister_chrdev_region(dev, 1);
        pr_info("Failed to add character device\n");
        return -1;
    }

    dev_class = class_create(THIS_MODULE, "Driven_class");
    if (IS_ERR(dev_class)) {
        pr_info("Cannot create struct class\n");
        unregister_chrdev_region(dev, 1);
        return -1;
    }

    if (IS_ERR(device_create(dev_class, NULL, dev, NULL, "Driven_class"))) {
        pr_info("Cannot create device\n");
        class_destroy(dev_class);
        unregister_chrdev_region(dev, 1);
        return -1;
    }

    kernel_buffer = kmalloc(MEM_SIZE, GFP_KERNEL);
    if (!kernel_buffer) {
        pr_info("Cannot allocate memory\n");
        class_destroy(dev_class);
        unregister_chrdev_region(dev, 1);
        return -1;
    }

    strcpy(kernel_buffer, "Hello world!!\n");
    pr_info("driver inserted successfully\n");

    return 0;
}

// Exit the module
static void __exit char1_exit(void) {
    kfree(kernel_buffer);
    class_destroy(dev_class);
    device_destroy(dev_class, dev);
    cdev_del(&file_cdev);
    unregister_chrdev_region(dev, 1);
    pr_info("Kernel module removed successfully\n");
}

module_init(char1_init);
module_exit(char1_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("GOKU");
MODULE_DESCRIPTION("Static numbers with IOCTL");
MODULE_VERSION("2.0");
