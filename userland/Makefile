
NAME=mymodule
PWD=${shell pwd}
SRCDIR=${PWD}/src
SRC=${SRCDIR}/main.c

CFLAGS=-Iinclude -Wall

BUILD_DIR=${PWD}/build


all: build

build: test_dir
	gcc -o ${BUILD_DIR}/${NAME} ${CFLAGS} ${SRC}

test_dir:
	@if [ ! -d "${BUILD_DIR}" ]; then     \
		echo "Build directory not exists, creating build dir";  \
		mkdir -p ${BUILD_DIR};      \
	fi

clean:
	@echo "Removing build files"
	@rm -r ${BUILD_DIR}
