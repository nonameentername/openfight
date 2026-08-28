CMAKE      := $(shell command -v cmake)
DOCKER     := $(shell command -v docker)
ARCH  	   := $(shell uname -m)
HOST_UID   := $(shell id -u)
HOST_GID   := $(shell id -g)
MAIN       := openfight
IMAGE_NAME := openfight-compiler
BUILD_TYPE ?= Release

build: clean
	${CMAKE} -Bbuild -DCMAKE_BUILD_TYPE=${BUILD_TYPE}
	${CMAKE} --build build

docker:
	${DOCKER} build . -t ${IMAGE_NAME}

all: ubuntu

docker-ubuntu:
	docker build -t godot-openfight-ubuntu ./platform/ubuntu

shell-ubuntu: docker-ubuntu
	docker run -it --rm --user ${HOST_UID}:${HOST_GID} -e HOME=/tmp -v ${CURDIR}:${CURDIR} -w ${CURDIR} godot-openfight-ubuntu ${SHELL_COMMAND}

ubuntu: ubuntu-debug ubuntu-release

ubuntu-debug:
	$(MAKE) shell-ubuntu SHELL_COMMAND='./platform/ubuntu/build_debug.sh'

ubuntu-release:
	$(MAKE) shell-ubuntu SHELL_COMMAND='./platform/ubuntu/build_release.sh'

xvfb-start:
	Xvfb $(DISPLAY) -screen 0 1024x768x24 > /dev/null 2>&1 &

xvfb-stop:
	pkill -9 Xvfb

ci:
	make clean build validate-log

validate-log:
	./$(MAIN) > log.txt 2>&1 &
	sleep 5
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

clean:
	rm -rf ${MAIN} *.exe *.o build cmake-build-* log.txt

check-leak:
	valgrind --leak-check=full --leak-check=full --show-leak-kinds=all --track-origins=yes ./${MAIN}

format:
	clang-format -i src/libopenfight/*.cpp src/libopenfight/*.h src/platform/sdl/*.cpp src/platform/sdl/*.h

format-check:
	clang-format -n src/libopenfight/*.cpp src/libopenfight/*.h src/platform/sdl/*.cpp src/platform/sdl/*.h
