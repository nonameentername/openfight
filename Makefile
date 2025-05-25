CMAKE      := $(shell command -v cmake)
MAIN       := openfight
IMAGE_NAME := openfight-compiler

compile: clean
	${CMAKE} -Bbuild -DCMAKE_BUILD_TYPE=Release
	${CMAKE} --build build
	
docker:
	docker build . -t ${IMAGE_NAME}

UNAME := $(shell uname)
ifeq ($(UNAME), Windows)
	UID=1000
	GID=1000
else
	UID=`id -u`
	GID=`id -g`
endif

shell:	docker
	docker run -it --rm -v `pwd`:/tmp/workdir --user ${UID}:${GID} -w /tmp/workdir ${IMAGE_NAME} bash

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
