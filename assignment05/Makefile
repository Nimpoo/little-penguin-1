obj-m := hello_misc_device.o
PWD = $(shell pwd)
KDIR = /lib/modules/$(shell uname -r)/build

all :
	make -C $(KDIR) M=$(PWD) modules
clean : 
	make -C $(KDIR) M=$(PWD) clean 
