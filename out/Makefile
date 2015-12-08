PROGRAM=simulator 
PACK_NAME=02_xcadap00_xcadap00.tar.gz

COMPILATOR=g++
PARAMS="-std=c++11"

all: clean build

build:
	${COMPILATOR} $(PARAMS) *.cpp -o ${PROGRAM}
clean:
	rm -f *.o ${PROGRAM}
run:
	./${PROGRAM}
pack:
	tar -czf ./${PACK_NAME} *.h *.cpp Makefile *.pdf 