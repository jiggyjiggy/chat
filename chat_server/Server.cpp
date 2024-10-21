#include "Server.h"
#include "CommandManager.h"
#include "SessionManager.h"
#include "Session.h"

#include "CipherManager.h"

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

    unsigned char encrypted[256];
    unsigned char decrypted[256];
    size_t encrypted_len = 0, decrypted_len = 0;

    EVP_PKEY* keypair = CipherManager::create_RSA_keypair();

    CipherManager::save_keys(keypair);

    // 공개 키를 PEM 형식으로 직렬화
    BIO* bio = BIO_new(BIO_s_mem());
    PEM_write_bio_PUBKEY(bio, keypair);

    // PEM 데이터를 메모리에 저장
    char* pem_key = nullptr;
    long pem_len = BIO_get_mem_data(bio, &pem_key);

    // 자원 해제
    BIO_free(bio);

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

        // 클라이언트에게 공개 키 전송
        if (send(socketFd, pem_key, pem_len, 0) == -1) {
            perror("send");
        }

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
