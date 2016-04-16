BIN=mymodule
NODE=mymodule
KDIR=/lib/modules/$(shell uname -r)/build
PWD=$(shell pwd)

obj-m := ${BIN}.o

all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean

install:
	sudo insmod ./${BIN}.ko
	sudo lsmod | grep '${BIN}'
	@echo "Install the file node"
	mknod /dev/${NODE} c 240 1
	chown root /dev/${NODE}
	chmod 0666 /dev/${NODE}
	chgrp plugdev /dev/${NODE}
	ls -al /dev/${NODE}

remove:
	sudo rmmod xpcie
	rm -rf /dev/${NODE}
