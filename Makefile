CXX = clang++
CC = clang
CXX_FLAGS = -g -std=c++11 -pedantic -Wall -Wextra -I. -I/home/mackenzie/include -L/home/mackenzie/lib

bitvector: bitvector.cpp bitvector.hpp
	$(CXX) bitvector.cpp -o bitvector $(CXX_FLAGS) -lncurses

bbhash: bbhash.cpp bbhash.hpp xxhash.h xxhash.o
	$(CXX) bbhash.cpp xxhash.o -o bbhash $(CXX_FLAGS) -lncurses -lsdsl

xxhash.o: xxhash.c
	$(CC) -c xxhash.c -o xxhash.o

clean:
	rm -rf *.o bbhash bitvector

