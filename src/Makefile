TARGET := mymodule
KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

obj-m := mymodule.o
$(TARGET)-objs := 

all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean

install:
	sudo insmod ./${TARGET}.ko

remove:
	sudo rmmod ${TARGET}

