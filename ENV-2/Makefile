OBJ = main.o Complex.o Sort.o
TARGET = complex_sort

all: $(TARGET)

$(TARGET): $(OBJ)
	g++ -o $(TARGET) $(OBJ)

main.o: main.cpp Complex.h Sort.h
	g++ -c main.cpp

Complex.o: Complex.cpp Complex.h
	g++ -c Complex.cpp

Sort.o: Sort.cpp Sort.h Complex.h
	g++ -c Sort.cpp

clean:
	rm -f $(OBJ) $(TARGET)

