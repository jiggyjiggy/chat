#ifndef CLIENT_H
#define CLIENT_H

#include <string>

class Client
{
public:
	Client(const std::string& host, int port);
	
	void start();
	void close();

private:
	std::string mHost;
	int mPort;
	int mSocketFd;
	bool mbClosed;
};

#endif /* CLIENT_H */