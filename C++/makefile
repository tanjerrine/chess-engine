CXX = g++
CXXFLAGS = -Wall -std=c++11 -pedantic -g -lm

all: main

main: main.o board.o move.o utils.o strategy.o
	${CXX} -o $@ ${CXXFLAGS} $^

main.o: main.cpp

board.o: board.cpp

move.o: move.cpp

strategy.o: strategy.cpp

utils.o: utils.cpp

clean:
	rm -f main main.o board.o move.o utils.o strategy.o
