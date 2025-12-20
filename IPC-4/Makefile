CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -pthread

CLIENT = client
SERVER = server

all: $(CLIENT) $(SERVER)

$(CLIENT): client.cpp
	$(CXX) $(CXXFLAGS) client.cpp -o $(CLIENT)

$(SERVER): server.cpp
	$(CXX) $(CXXFLAGS) server.cpp -o $(SERVER)

clean:
	rm -f $(CLIENT) $(SERVER)

.PHONY: all clean
