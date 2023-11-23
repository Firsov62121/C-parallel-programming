
GCC = g++
objs := main.o
debugFlags = -O0 -lstdc++ -Wall -ggdb3 -lncurses -std=c++17
releaseFlags = -O3 -lncurses -std=c++17

all: debug

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
	@rm -f $(objs) main