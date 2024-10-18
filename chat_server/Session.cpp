#include "Session.h"
#include "SessionManager.h"
#include "CommandManager.h"
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <thread>

#include <cstring>

Session::Session(int socketFd, CommandManager& commandManager, SessionManager& sessionManager)
	: mSocketFd(socketFd)
	, mCommandManager(&commandManager)
	, mSessionManager(&sessionManager)
	, mbClosed(false)
{
	mSessionManager->add(this);
}

Session::~Session()
{
	close();
}

void Session::run()
{
    std::cout << "Session::run() 시작" << std::endl;

    char buffer[1024];
    try {
        while (true) {
            int bytesReceived = recv(mSocketFd, buffer, sizeof(buffer), 0);

            if (bytesReceived < 0) // 오류 발생
			{
                std::cerr << "[" << std::this_thread::get_id() << "] recv 실패: " << strerror(errno) << ", 소켓 FD: " << mSocketFd << std::endl;
                break;  
            } 
			else if (bytesReceived == 0) // 연결 종료
			{
                std::cout << "클라이언트 연결 종료, 소켓 FD: " << mSocketFd << std::endl;
                break;
            }
		
			std::string received(buffer, bytesReceived);
			std::cout << "client -> server: " << received << ", 소켓 FD: " << mSocketFd << std::endl;
			mCommandManager->execute(received, *this);
        }
    } 
	catch (const std::exception& e) 
	{
        std::cerr << "Exception: " << e.what() << ", 소켓 FD: " << mSocketFd << std::endl;
    }

    // 세션 종료 처리
    std::cout << "세션 종료, 소켓 FD: " << mSocketFd << std::endl;
    mSessionManager->remove(*this);
	mSessionManager->sendAll(mUsername + "님이 퇴장했습니다.");

    close();  // 소켓 닫기
    std::cout << "소켓 닫힘, 소켓 FD: " << mSocketFd << std::endl;
}


void Session::send(const std::string& message)
{
    std::cout << "server -> client: " << message << " (size: " << message.size() << ")" << std::endl;
    std::cout << "mSocketFd:  " << mSocketFd << std::endl;

    if (::send(mSocketFd, message.c_str(), message.size(), 0) == -1) 
	{
        throw std::runtime_error("Error sending message");
    }
}

void Session::close()
{
	if (mbClosed == true)
	{
		return;
	}

	::close(mSocketFd);
	mbClosed = true;
}

std::string& Session::getUsername()
{
	return mUsername;
}

void Session::setUsername(std::string& username)
{
	mUsername = username;
}

bool Session::operator==(const Session& other) const
{
	return (mSocketFd == other.mSocketFd); // 임시
}
