#include "ReadHandler.h"
#include "Client.h"
#include <cstring>
#include <unistd.h>
#include <iostream>

ReadHandler::ReadHandler(int socketFd, Client& client)
	: mSocketFd(socketFd)
	, mClient(client)
	, mbClosed(false)
{
}

void ReadHandler::run()
{
	char buffer[1024];
	while (mbClosed == false)
	{
		std::cout << "ReadHandler::run() mSocketFd : " << mSocketFd << std::endl;
		size_t bytesReceived = read(mSocketFd, buffer, sizeof(buffer));
		if (bytesReceived <= 0)
		{
			std::cerr << "서버 연결 종료" << std::endl;
			mClient.close();
			break;
		}

		std::cout << "Received: " << buffer << std::endl;
	}
	std::cout << "ReadHandler::run() 종료" << std::endl;
}