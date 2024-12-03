obj-m := ioctl.o

# Native Compilation

KERN_DIR := /lib/modules/$(shell uname -r)/build

PWD := $(shell pwd)

default :
	$(MAKE) -C $(KERN_DIR) M=$(PWD) modules
	gcc -o user_app user_app.c
clean	:
	$(MAKE) -C $(KERN_DIR) M=$(PWD) clean
	rm -rf user_app