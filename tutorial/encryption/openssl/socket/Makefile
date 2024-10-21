# Variables for programs and paths
CXX = g++
CXXFLAGS = -Wall -O2

# Use pkg-config to find OpenSSL paths dynamically
OPENSSL_CFLAGS = $(shell pkg-config --cflags openssl)
OPENSSL_LIBS = $(shell pkg-config --libs openssl)

# Source files and targets
CLIENT_SRC = client.cpp
SERVER_SRC = server.cpp
CLIENT_BIN = client
SERVER_BIN = server

# Default target
all: $(CLIENT_BIN) $(SERVER_BIN)

# Client target
$(CLIENT_BIN): $(CLIENT_SRC)
	$(CXX) $(CXXFLAGS) $(OPENSSL_CFLAGS) -o $@ $^ $(OPENSSL_LIBS)

# Server target
$(SERVER_BIN): $(SERVER_SRC)
	$(CXX) $(CXXFLAGS) $(OPENSSL_CFLAGS) -o $@ $^ $(OPENSSL_LIBS)

# Clean up compiled files
clean:
	rm -f $(CLIENT_BIN) $(SERVER_BIN)

# Check if OpenSSL is installed, and install it if missing
openssl_check:
	@which openssl > /dev/null || (echo "OpenSSL not found. Installing..." && brew install openssl)

# Target to check for OpenSSL and then build
build: openssl_check all
