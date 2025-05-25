CMAKE      := $(shell command -v cmake)
DOCKER     := $(shell command -v docker)
MAIN       := openfight
IMAGE_NAME := openfight-compiler
BUILD_TYPE ?= Release

compile: clean
	${CMAKE} -Bbuild -DCMAKE_BUILD_TYPE=${BUILD_TYPE}
	${CMAKE} --build build
	
docker:
	${DOCKER} build . -t ${IMAGE_NAME}

UNAME := $(shell uname)
ifeq ($(UNAME), Windows)
	UID=1000
	GID=1000
else
	UID=`id -u`
	GID=`id -g`
endif

shell:	docker
	${DOCKER} run -it --rm -v `pwd`:/tmp/workdir --user ${UID}:${GID} -w /tmp/workdir ${IMAGE_NAME} bash

zip:
	zip -r ${MAIN}-${PLATFORM}.zip data ${MAIN} ${MAIN}.exe

linux:
	PLATFORM=linux CXX=g++ make compile	zip

windows:
	PLATFORM=windows CXX=i686-w64-mingw32-g++ make compile zip

mac:
	PLATFORM=mac CXX=clang make compile zip	

clean:
	rm -rf ${MAIN} *.exe *.o build/*

check-leak:
	valgrind --leak-check=full --leak-check=full --show-leak-kinds=all  --track-origins=yes ./openfight