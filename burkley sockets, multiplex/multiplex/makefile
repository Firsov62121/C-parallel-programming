
GCC = g++
all: build_server

build_server:
	$(GCC) -std=c++11 -o server server.cpp
poll:
	$(GCC) -std=c++11 -o server poll_server.cpp
epoll:
	$(GCC) -std=c++11 -o server epoll_server.cpp

.PHONY : clean

clean:
	@rm -f server