
PWD=$(shell pwd)
MODULEDIR := $(PWD)/src
USERDIR := $(PWD)/userland
RULES_FILE=61-mymodule.rules

all: user MODULE

MODULE:
	make -C $(MODULEDIR)

instrules:
	sudo cp rules/${RULES_FILE} /etc/udev/rules.d/

install: instrules
	@printf "Installing Driver"
	make -C $(MODULEDIR) install

rmrules:
	sudo rm /etc/udev/rules.d/${RULES_FILE}

remove: rmrules
	make -C $(MODULEDIR) remove

clean:
	make -C $(MODULEDIR) clean

user:
	make -C $(USERDIR)

