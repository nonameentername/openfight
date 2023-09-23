MAIN=openfight
IMAGE_NAME=openfight-compiler

docker:
	docker build . -t ${IMAGE_NAME}

shell:	docker
	docker run -it --rm -v `pwd`:/tmp/workdir -w /tmp/workdir ${IMAGE_NAME} bash

compile: clean
	mkdir -p build \
	&& cd build \
	&& cmake .. \
	&& make \
	&& cp ${MAIN}* ..

zip:
	zip -r ${MAIN}-${PLATFORM}.zip data images ${MAIN} ${MAIN}.exe

linux:
	PLATFORM=linux CXX=g++ make compile	zip

windows:
	PLATFORM=windows CXX=i686-w64-mingw32-g++ make compile zip

mac:
	PLATFORM=mac CXX=clang make compile zip	

clean:
	rm -rf ${MAIN} *.exe *.o build/*