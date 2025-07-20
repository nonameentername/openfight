CMAKE      := $(shell command -v cmake)
DOCKER     := $(shell command -v docker)
ARCH  	   := $(shell uname -m)
MAIN       := openfight
IMAGE_NAME := openfight-compiler
BUILD_TYPE ?= Release

build: clean
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

ubuntu-install-deps:
	sudo apt install -y build-essential libsdl2-dev libsdl2-image-dev libglu1-mesa-dev libglew-dev nlohmann-json3-dev

macos-install-deps:
	brew install sdl2 sdl2_image sdl2_gfx cmake make glew nlohmann-json

shell:	docker
	${DOCKER} run -it --rm -v `pwd`:/tmp/workdir --user ${UID}:${GID} -w /tmp/workdir ${IMAGE_NAME} bash

zip:
	zip -9 -r ${MAIN}-${PLATFORM}-${ARCH}.zip data ${MAIN} ${MAIN}.exe

linux:
	PLATFORM=linux CXX=g++ make build zip

windows:
	PLATFORM=windows CXX=i686-w64-mingw32-g++ CC=i686-w64-mingw32-gcc make build zip

mac:
	PLATFORM=mac CXX=g++ make build zip	

clean:
	rm -rf ${MAIN} *.exe *.o build

check-leak:
	valgrind --leak-check=full --leak-check=full --show-leak-kinds=all --track-origins=yes ./${MAIN}