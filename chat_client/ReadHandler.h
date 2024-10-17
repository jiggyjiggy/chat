#ifndef READ_HANDLER_H
#define READ_HANDLER_H

#include "Client.h"
class Client;

class ReadHandler
{
public:
	ReadHandler(int socketFd, Client& client);

	void run();

private:
	int mSocketFd;
	Client& mClient;
	bool mbClosed;
};

#endif /* READ_HANDLER_H */