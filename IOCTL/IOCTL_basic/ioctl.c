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

dev_t dev = 0;
static struct cdev my_dev;

static int __init char_driver_init(void);
static void __exit char_driver_exit(void);

static int my_open(struct inode *inode, struct file *file);
static int my_release(struct inode *inode, struct file *file);
static ssize_t my_read(struct file *filp, char __user *buf, size_t len, loff_t *off);
static ssize_t my_write(struct file *filp, const char __user *buf, size_t len, loff_t *off);
static long my_ioctl(struct file *file, unsigned int cmd, unsigned long arg);

static struct file_operations fops =
{
	.owner		= THIS_MODULE,
	.read		= my_read,
	.write		= my_write,
	.open		= my_open,
	.release 	= my_release, 
	.unlocked_ioctl	= my_ioctl, 
};

static int my_open(struct inode *inode, struct file *file)
{
	pr_info("\n Device file Opened\n");
	return 0;
}

static int my_release(struct inode *inode, struct file *file)
{
	pr_info("\n Device file Closed\n");
	return 0;
}

static ssize_t my_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
	pr_info("\n Driver Read Done!\n");
	return mem_size;
}

static ssize_t my_write(struct file *filp, const char __user *buf, size_t len, loff_t *off)
{
	pr_info("\n Driver Write Done!\n");
	return len;
}

static long my_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	pr_info("\n IOCTL invoked\n");
	switch(cmd)
	{
		case CMD_1 :
			pr_info("\nThis is CMD_1 command\n");
			break;
		case CMD_2 :
			pr_info("\nThis is CMD_2 command .. Received arg = %d\n", arg);
			break;
		default :
			pr_info("\n Invalid Command\n");
			break;
	}
	return 0;
}


static int __init char_driver_init(void)
{
	if(alloc_chrdev_region(&dev, 0, 1, "Mydev") < 0)
	{
		pr_err("\n Error in allocating Major Number\n");
		return -1;
	}
	pr_info("\n Major = %d and Minor = %d\n", MAJOR(dev), MINOR(dev));

	cdev_init(&my_dev, &fops);
	if(cdev_add(&my_dev, dev, 1) < 0)
	{
		pr_err("\n Error in adding device \n");
		return -1;
	}
	return 0;
}

static void __exit char_driver_exit(void)
{
	cdev_del(&my_dev);
	unregister_chrdev_region(dev, 1);
	pr_info("\n Driver Removed Successfully\n");
}

module_init(char_driver_init);
module_exit(char_driver_exit);
