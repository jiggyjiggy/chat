#include "ReadHandler.h"
#include "Client.h"
#include "MyRSA.h"
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
	// 클라이언트의 개인키를 사용해 복호화할 MyRSA 객체 생성
	MyRSA rsa("private.pem", PRIVATE_KEY);  // 클라이언트의 개인키
	std::cout << "after MyRSA rsa(private.pem);" << std::endl;
	
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

		// 암호화된 데이터를 복호화할 버퍼
		unsigned char decrypted[1024];  
		size_t decrypted_len = 0;

		// 복호화 실행
		rsa.decrypt((unsigned char*)buffer, bytesReceived, decrypted, decrypted_len);

		// 복호화된 메시지를 출력
		std::cout << "Received (decrypted): " << std::string((char*)decrypted, decrypted_len) << std::endl;
	}
	std::cout << "ReadHandler::run() 종료" << std::endl;
}
