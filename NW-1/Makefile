CXX = g++
CXXFLAGS = -std=c++17 -pthread -Wall -O2

SERVER_SOURCES = Socket.cpp ThreadPool.cpp TcpServer.cpp server.cpp
CLIENT_SOURCES = Socket.cpp client.cpp

all: server client

server: $(SERVER_SOURCES)
	$(CXX) $(CXXFLAGS) $^ -o $@

client: $(CLIENT_SOURCES)
	$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	rm -f server client
