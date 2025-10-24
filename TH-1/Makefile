CXX = g++
CXXFLAGS = -std=c++17 -O2 -pthread
TARGET = array-summary

all: $(TARGET)

$(TARGET): th-2.cpp
        $(CXX) $(CXXFLAGS) th-2.cpp -o $(TARGET)

clean:
        rm -f $(TARGET)                     
