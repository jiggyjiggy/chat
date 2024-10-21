#include "Session.h"
#include "SessionManager.h"
#include "CommandManager.h"

#include "MyRSA.h"

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
	char client_pubkey_buffer[4096];
	int received = recv(mSocketFd, client_pubkey_buffer, sizeof(client_pubkey_buffer), 0);
    if (received < 0)
    {
        perror("recv");
    }
    // 클라이언트의 MyRsa 공개 키 불러오기
	std::string client_Pubkey = std::to_string(mSocketFd) + "_public.pem";
    FILE* pub_file = fopen(client_Pubkey.c_str(), "w");
    size_t written = fwrite(client_pubkey_buffer, 1, received, pub_file);
    EVP_PKEY* client_pub_key = PEM_read_PUBKEY(pub_file, NULL, NULL, NULL);

	mSessionManager->add(this);
}

Session::~Session()
{
    // EVP_PKEY_free(bub_file);
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

            MyRSA rsa("private.pem");  // 클라이언트의 개인키를 로드
			// 암호화된 데이터를 복호화할 버퍼
            unsigned char decrypted[1024];  
            size_t decrypted_len = 0;

            // 복호화 실행
            rsa.decrypt((unsigned char*)buffer, bytesReceived, decrypted, decrypted_len);

		    // 복호화된 메시지를 출력
		    std::cout << "Received (decrypted): " << std::string((char*)decrypted, decrypted_len) << std::endl;
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

    // 서버의 공개키를 이용해 메시지를 암호화할 MyRSA 객체 생성
	std::string client_Pubkey = std::to_string(mSocketFd) + "_public.pem";
	MyRSA rsa(client_Pubkey);

	// 암호화된 메시지를 저장할 버퍼
	unsigned char encrypted[256];  
	size_t encrypted_len = 0;

	// 메시지를 RSA 공개키로 암호화
	rsa.encrypt((unsigned char*)message.c_str(), encrypted, encrypted_len);

	// 암호화된 메시지를 서버로 전송
    if (::send(mSocketFd, encrypted, encrypted_len, 0) == -1) 
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
