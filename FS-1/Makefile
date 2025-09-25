C = g++
CFLAGS = -Wall -O2

TARGET = read-file

all: $(TARGET)

$(TARGET): read-file.o

	$(C) $(FLAGS) -o $(TARGET) read-file.o

read-file.o: read-file.cpp
	$(C) $(CFLAGS) -c read-file.cpp

clean: 
	rm -f $(TARGET) *.o
