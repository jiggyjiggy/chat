#include "Server.h"
#include "CommandManager.h"
#include "SessionManager.h"
#include "Session.h"
#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>

#include <cstring>

Server::Server(int port, CommandManager& commandManager, SessionManager& sessionManager)
	: mPort(port)
	, mCommandManager(commandManager)
	, mSessionManager(sessionManager)
	, mServerSocketFd(-1)
    , mPool(5)
{
}

void Server::start() {
    std::cout << "서버 시작: " << typeid(&mCommandManager).name() << std::endl; 

	// 서버 소켓 생성
	mServerSocketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (mServerSocketFd == -1) {
        std::cerr << "소켓 생성 실패: " << strerror(errno) << std::endl;
        throw std::runtime_error("소켓 생성 실패");
    }
	std::cout << "소켓 생성 성공" << std::endl;

	sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(mPort);

	// bind
    if (bind(mServerSocketFd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) 
	{
        std::cerr << "bind 실패: " << strerror(errno) << std::endl;
        ::close(mServerSocketFd);
        throw std::runtime_error("bind 실패");
    }
	std::cout << "bind 성공" << std::endl;

	// listen
	if (listen(mServerSocketFd, 3) == -1) 
	{
        std::cerr << "listen 실패: " << strerror(errno) << std::endl;
        close(mServerSocketFd);
        throw std::runtime_error("listen 실패");
    }
	std::cout << "listen 성공" << std::endl;

	std::cout << "서버 소켓 시작 - 리스닝 포트: " << mPort << std::endl;

    running();
}

void Server::running() {

    while (true) {
		sockaddr_in clientAddr{};
    	socklen_t clientAddrLen = sizeof(clientAddr);

        int socketFd = accept(mServerSocketFd, (struct sockaddr*)&clientAddr, (socklen_t*)&clientAddrLen);
        if (socketFd == -1) {
            std::cerr << "클라이언트 연결 실패: " << strerror(errno) << std::endl;
            continue;
        }

		char clientIP[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, sizeof(clientIP));
        std::cout << "클라이언트 IP: " << clientIP << ", 소켓 연결: " << socketFd << std::endl;


		Session* session = new Session(socketFd, mCommandManager, mSessionManager);

        
        /*
		std::thread sessionThread([session]() {
			std::cout  << "새로운 스레드 시작. 스레드 ID: " << std::this_thread::get_id() << std::endl;
            session->run();
            delete session; // 세션 종료 후 메모리 해제
        });
        sessionThread.detach();
        */
            mPool.enqueue([session]() {
                std::cout << "새로운 세션을 위한 작업 시작. 스레드 ID: " << std::this_thread::get_id() << std::endl;
                session->run(); // 세션 작업 실행
                delete session; // 세션 종료 후 메모리 해제
        });
    }

    std::this_thread::sleep_for(std::chrono::seconds(10));
}
