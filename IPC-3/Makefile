all: first second

first: first.cpp
	g++ first.cpp -o first -lrt -lpthread

second: second.cpp
	g++ second.cpp -o second -lrt -lpthread

clean:
	rm -rf *.o first second
