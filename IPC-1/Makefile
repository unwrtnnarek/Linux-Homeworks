C=g++
CFLAGS= -c

all:signal-echo

signal-echo:main.o
	$(C) $^ -o signal-echo

main.o:main.cpp
	$(C) $(CFLAGS) $^ -o signal-echo


clean:
	rm -rf *.o signal-echo
