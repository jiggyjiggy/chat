#ifndef WRITE_HANDLER_H
#define WRITE_HANDLER_H

#include <string>

#include "Client.h"
class Client;

class WriteHandler
{
public:
	WriteHandler(const int socketFd, Client& client);

	void run();

private:
	std::string inputUsername();
	void sendToServer(const std::string& message);

	int mSocketFd;
	Client& mClient;
	bool mbClosed;
};

#endif /* WRITE_HANDLER_H */