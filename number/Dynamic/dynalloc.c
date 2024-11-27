#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
//#include <linux/init.h>

dev_t dev=100;


static int __init char_init(void){
    alloc_chrdev_region(&dev,0,1,"character type\n");
    printk(KERN_INFO"Device MINOR = %d , MAJOR=%d\n",MAJOR(dev),MINOR(dev));
    return 0;
}

static void __exit char_exit(void){
    unregister_chrdev_region(dev,1);
    printk(KERN_INFO "Device removed\n");
}

module_init(char_init);
module_exit(char_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("GOKU");
MODULE_DESCRIPTION("TESTING");

