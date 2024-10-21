# Variables for programs and paths
CXX = g++
CXXFLAGS = -Wall -O2

# Use pkg-config to find OpenSSL paths dynamically
OPENSSL_CFLAGS = $(shell pkg-config --cflags openssl)
OPENSSL_LIBS = $(shell pkg-config --libs openssl)

# AES and RSA source files and targets
AES_SRC = AES.cpp
RSA_SRC = RSA.cpp
AES_BIN = aes
RSA_BIN = rsa

# Default target
all: $(AES_BIN) $(RSA_BIN)

# AES target
$(AES_BIN): $(AES_SRC)
	$(CXX) $(CXXFLAGS) $(OPENSSL_CFLAGS) -o $@ $^ $(OPENSSL_LIBS)

# RSA target
$(RSA_BIN): $(RSA_SRC)
	$(CXX) $(CXXFLAGS) $(OPENSSL_CFLAGS) -o $@ $^ $(OPENSSL_LIBS)

# Clean up compiled files
clean:
	rm -f $(AES_BIN) $(RSA_BIN)

# Check if OpenSSL is installed, and install it if missing
openssl_check:
	@which openssl > /dev/null || (echo "OpenSSL not found. Installing..." && brew install openssl)

# Target to check for OpenSSL and then build
build: openssl_check all
