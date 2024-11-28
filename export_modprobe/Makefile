obj-m += main.o add.o mul.o sub.o div.o
kern= /lib/modules/$(shell uname -r)/extra

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) clean

install:
	sudo mkdir -p $(kern)
	sudo cp *.ko $(kern)
	sudo depmod -a
