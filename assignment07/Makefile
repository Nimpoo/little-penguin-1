obj-m := assignment07.o
assignment07-objs := main.o id.o jiffies.o foo.o
PWD = $(shell pwd)
KDIR = /lib/modules/$(shell uname -r)/build

all :
	make -C $(KDIR) M=$(PWD) modules
clean : 
	make -C $(KDIR) M=$(PWD) clean 
