#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/err.h>
#include <linux/cdev.h>
#include <linux/device.h>

dev_t dev = 0;
static struct cdev file_cdev;

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

static int FILE_OPEN(struct inode *inode, struct file *file)
{
    pr_info("driver release successfully\n");
    return 0;
}

static int FILE_RELEASE(struct inode *inode, struct file *file)
{
    pr_info("driver release successfully\n");
    return 0;
}

static ssize_t FILE_READ(struct file *file, char __user *buff, size_t len, loff_t *off)
{
    pr_info("driver read successfully\n");
    return 0;  }

static ssize_t FILE_WRITE(struct file *file, const char __user *buff, size_t len, loff_t *off)
{
    pr_info("driver write successfully\n");
    return len;  
}
static int __init char1_init(void)
{
    if (alloc_chrdev_region(&dev, 0, 1, "character device 35") < 0) {
        printk(KERN_INFO"Character device allocation failed\n");
        return -1;
    }
    printk(KERN_INFO "Major=%d, Minor=%d\n", MAJOR(dev), MINOR(dev));

    cdev_init(&file_cdev, &fops);

    if (cdev_add(&file_cdev, dev, 1) < 0) {
        unregister_chrdev_region(dev, 1);
        printk(KERN_INFO"Failed to add character device\n");
        return -1;
    }

    printk(KERN_INFO"Kernel module inserted successfully\n");
    return 0;
}

static void __exit char1_exit(void)
{
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
