#include<linux/types.h>
#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>

int div(int a,int b){
    return a/b;
}
EXPORT_SYMBOL(div);

static int __init char_init(void){
    printk(KERN_INFO "Added function\n");
    return 0;
}

static void __exit char_exit(void){
    printk(KERN_INFO "Removed function\n");
}

module_init(char_init);
module_exit(char_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("GOKU");
MODULE_DESCRIPTION("TESTING");


