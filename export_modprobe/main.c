#include<linux/types.h>
#include<linux/init.h>
#include<linux/kernel.h>
#include<linux/module.h>
#include"add.h"

static int a=35;
static int b=15;
static int __init char_init(void){
    int r=add(a,b);
    printk(KERN_INFO "Result is %d\n",r);
    r=sub(a,b);
    printk(KERN_INFO "Result is %d\n",r);
    r=mul(a,b);
    printk(KERN_INFO "Result is %d\n",r);
    r=div(a,b);
    printk(KERN_INFO "Result is %d\n",r);
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


