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

#define MEM_SIZE 1024

dev_t dev = 0;
static struct cdev file_cdev;
static char *kernel_buffer;
static struct class *dev_class; 

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
};

static int FILE_OPEN(struct inode *inode, struct file *file) {
    pr_info("driver open successfully\n");
    return 0;
}

static int FILE_RELEASE(struct inode *inode, struct file *file) {
    pr_info("driver release successfully\n");
    return 0;
}

static ssize_t FILE_READ(struct file *file, char __user *buff, size_t len, loff_t *off) {
    if (copy_to_user(buff, kernel_buffer, MEM_SIZE)) {
        pr_err("Error in data read!!!\n");
        return -1;
    }
    pr_info("driver read successfully\n");
    return MEM_SIZE;
}

static ssize_t FILE_WRITE(struct file *file, const char __user *buff, size_t len, loff_t *off) {
    if (copy_from_user(kernel_buffer, buff, len)) {
        pr_err("Error in data write!!!\n");
        return -EFAULT;
    }
    pr_info("driver write successfully\n");
    return len;
}


static int __init char1_init(void) {
    if (alloc_chrdev_region(&dev, 0, 1, "character_device") < 0) {
        printk(KERN_INFO "Character device allocation failed\n");
        return -1;
    }
    printk(KERN_INFO "Major=%d, Minor=%d\n", MAJOR(dev), MINOR(dev));

    cdev_init(&file_cdev, &fops);

    if (cdev_add(&file_cdev, dev, 1) < 0) {
        unregister_chrdev_region(dev, 1);
        printk(KERN_INFO "Failed to add character device\n");
        return -1;
    }

    dev_class = class_create(THIS_MODULE, "Driven_class");
    if (IS_ERR(dev_class)) {
        pr_info("Cannot Create Struct class\n");
        unregister_chrdev_region(dev, 1);
        return -1;
    }

    if (IS_ERR(device_create(dev_class, NULL, dev, NULL, "Driven_class"))) {
        pr_info("Cannot Create Device\n");
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


static void __exit char1_exit(void) {
    kfree(kernel_buffer);
    class_destroy(dev_class);
    device_destroy(dev_class, dev);
    cdev_del(&file_cdev);
    unregister_chrdev_region(dev, 1);
    printk(KERN_INFO "Kernel module removed successfully\n");
}

module_init(char1_init);
module_exit(char1_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("GOKU");
MODULE_DESCRIPTION("Static numbers");
MODULE_VERSION("2.0");
