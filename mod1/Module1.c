#include<linux/types.h>
#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>
static int __init char_init(void){
    printk(KERN_INFO "ON\n");
    return 0;
}

static void __exit char_exit(void){
    printk(KERN_INFO "OFF\n");
}

module_init(char_init);
module_exit(char_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("GOKU");
MODULE_DESCRIPTION("TESTING");