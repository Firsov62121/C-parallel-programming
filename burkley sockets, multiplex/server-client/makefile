
GCC = gcc
objs := server.o client.o
debugFlags = -O0 -lstdc++ -Wall -ggdb3 -lncurses -std=c++17
releaseFlags = -O3 -lncurses -std=c++17

all: build_server build_client

build_server:
	gcc -ggdb -O0 -o server server.c
build_client:
	gcc -ggdb -O0 -o client client.c

valgrind:
	valgrind --leak-check=full --track-origins=yes ./main

release: $(objs)
	$(GCC) -o main $(objs) $(releaseFlags)

debug: $(objs)
	$(GCC) -o main $(objs) $(debugFlags)

%.o: %.cpp
	$(GCC) $(debugFlags) -c -o $@ $<

.PHONY : clean

clean:
	@rm -f server client