#include "WriteHandler.h"
#include "Client.h"
#include <iostream>
#include <unistd.h>

WriteHandler::WriteHandler(int socketFd, Client& client)
	: mSocketFd(socketFd)
	, mClient(client)
	, mbClosed(false)
{
}

void WriteHandler::run()
{
	const std::string& DELIMITER = "|";
	const std::string username = inputUsername(); // 대기!
	sendToServer("/join" + DELIMITER + username);

	std::string toSend;
	while (mbClosed == false)
	{
		std::cout << "WriteHandler::run() mSocketFd : " << mSocketFd << std::endl;

		std::getline(std::cin, toSend); // 블로킹 입력

		if (toSend.empty())
		{
			continue;
		}

		if (toSend == "/exit")
		{
			sendToServer(toSend);
			break;
		}

		// "/"로 시작하면 명령어, 나머지는 일반 메시지
		if (toSend[0] == '/')
		{
			sendToServer(toSend);
		}
		else
		{
			sendToServer("/message" + DELIMITER + toSend);
		}
	}

	mClient.close();
}

std::string WriteHandler::inputUsername()
{
	std::string username;
	std::cout << "이름을 입력하세요" << std::endl;
	do {
		std::getline(std::cin, username);
	} while (username.empty());

	std::cout << "이름: " << username << std::endl;
	return username;
}

void WriteHandler::sendToServer(const std::string& message)
{
	size_t bytesSent = write(mSocketFd, message.c_str(), message.size());
	if (bytesSent < 0)
	{
		std::cerr << "서버로 데이터 전송 실패" << std::endl;
		mClient.close();
	}
}