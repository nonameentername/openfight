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
	sudo apt install -y build-essential libsdl2-dev libsdl2-image-dev libglu1-mesa-dev libglew-dev libyaml-cpp-dev xvfb

macos-install-deps:
	brew install sdl2 sdl2_image sdl2_gfx cmake make glew yaml-cpp

shell:	docker
	${DOCKER} run -it --rm -v `pwd`:/tmp/workdir --user ${UID}:${GID} -w /tmp/workdir ${IMAGE_NAME} bash

ci-docker:
	${DOCKER} build -t ${IMAGE_NAME} .
	${DOCKER} run --rm -v "$(PWD)":/app -w /app ${IMAGE_NAME} sh -c "make clean build validate-log"

xvfb-start:
	Xvfb $(DISPLAY) -screen 0 1024x768x24 > /dev/null 2>&1 &

xvfb-stop:
	pkill -9 Xvfb

ci:
	make clean build validate-log

validate-log:
	./$(MAIN) > log.txt 2>&1 &
	sleep 3
	@if grep -q 'VideoSystem Init OK' log.txt; then \
		echo "✅ $(MAIN) ran successfully"; \
	else \
		echo "❌ $(MAIN) did not produce expected output"; \
		echo "----- log.txt -----"; \
		cat log.txt; \
		exit 1; \
	fi
	pkill -9 $(MAIN)

zip:
	zip -9 -r ${MAIN}-${PLATFORM}-${ARCH}.zip data ${MAIN} ${MAIN}.exe

linux:
	PLATFORM=linux CXX=g++ make build zip

windows:
	PLATFORM=windows CXX=i686-w64-mingw32-g++ CC=i686-w64-mingw32-gcc make build zip

mac:
	PLATFORM=mac CXX=g++ make build zip	

clean:
	rm -rf ${MAIN} *.exe *.o build cmake-build-* log.txt

check-leak:
	valgrind --leak-check=full --leak-check=full --show-leak-kinds=all --track-origins=yes ./${MAIN}

format:
	clang-format -i src/*.cpp include/*.h

format-check:
	clang-format -n src/*.cpp include/*.h