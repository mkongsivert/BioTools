CXX = clang++

CXX_FLAGS = -g -std=c++11 -pedantic -Wall -Wextra -I/home/mackenzie/include -I/home/mackenzie/vcpkg/packages/xxhash_x64-linux/include

bbhash: bbhash.cpp bbhash.hpp
	$(CXX) bbhash.cpp -o bbhash $(CXX_FLAGS) -lncurses

clean:
	rm -rf *.o bbhash