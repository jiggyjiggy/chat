#include "MyRSA.h"

#include "Client.h"
#include "ReadHandler.h"
#include "WriteHandler.h"

#include <iostream>
#include <arpa/inet.h>
#include <thread>
#include <unistd.h>

Client::Client(const std::string& host, int port)
	: mHost(host)
	, mPort(port)
	, mbClosed(false)
	, mSocketFd(-1)
{
}

/*
- inet_pton: 문자열 형식의 IP 주소를 네트워크 형식으로 변환하는 함수.
- htons: 호스트 바이트 순서를 네트워크 바이트 순서로 변환.
- sockaddr_in: IPv4 주소 정보를 저장하는 구조체.
- AF_INET: IPv4 인터넷 프로토콜을 의미하는 주소 패밀리.
*/
void Client::start()
{
	std::cout << "클라이언트 시작" << std::endl;

	// 소켓 생성
	mSocketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (mSocketFd < 0)
	{
		std::cerr << "소켓 생성 실패" << std::endl;
		return;
	}
	std::cout << "소켓 생성 성공" << std::endl;

	// 서버 주소 설정
	sockaddr_in server_addr = { 0, };
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(mPort); // htons(port)는 호스트 바이트 순서(Host Byte Order)를 네트워크 바이트 순서(Network Byte Order)로 바꿔주는 함수. 바이트 순서가 호스트와 네트워크 간에 다를 수 있으므로, 네트워크에서 사용할 수 있도록 포트를 변환.
	inet_pton(AF_INET, mHost.c_str(), &server_addr.sin_addr);

	// 서버에 연결
	if (connect(mSocketFd, (sockaddr*)&server_addr, sizeof(server_addr)) < 0)
	{
		std::cerr << "서버 연결 실패" << std::endl;
		return;
	}
	std::cout << "서버 연결 성공" << std::endl;





	// 서버의 공개키 받기, 파일로 저장(server_public.pem)
	char server_pubkey_buffer[4096];
    int received = recv(mSocketFd, server_pubkey_buffer, sizeof(server_pubkey_buffer), 0);

	std::cout << received << std::endl;

    if (received < 0)
    {
        std::cerr << "서버 공개키 수신 실패" << std::endl;
        return;
    }
    std::cout << "서버 공개키 수신 성공" << std::endl;

	// 서버의 MyRsa 공개 키 불러오기
    FILE* pub_file = fopen("server_public.pem", "w");
	size_t written = fwrite(server_pubkey_buffer, 1, received, pub_file);
	EVP_PKEY* server_pub_key = PEM_read_PUBKEY(pub_file, NULL, NULL, NULL);
	if (written != received)
	{
		std::cerr << "서버 공개키 파일 쓰기 실패" << std::endl;
		fclose(pub_file);
		return;
	}
	std::cout << "서버 공개키 파일 저장 성공" << std::endl;
	fclose(pub_file);


	// 클라이언트의 키페어 생성
	EVP_PKEY* keypair = MyRSA::create_RSA_keypair();
	MyRSA::save_keys(keypair);

	// 클라이언트의 공개키를 서버에 전송
	FILE* pub_file_client = fopen("public.pem", "r");
	if (!pub_file_client) {
		std::cerr << "클라이언트 공개키 파일 열기 실패" << std::endl;
		return;
	}
	fseek(pub_file_client, 0, SEEK_END);
	long pub_key_size = ftell(pub_file_client);
	rewind(pub_file_client);

	char* client_pubkey_buffer = new char[pub_key_size];
	fread(client_pubkey_buffer, 1, pub_key_size, pub_file_client);
	fclose(pub_file_client);

	if (send(mSocketFd, client_pubkey_buffer, pub_key_size, 0) < 0) {
		std::cerr << "클라이언트 공개키 전송 실패" << std::endl;
		delete[] client_pubkey_buffer;
		return;
	}
	std::cout << "클라이언트 공개키 전송 성공" << std::endl;

	delete[] client_pubkey_buffer;






	// read 스레드
	ReadHandler readHandler(mSocketFd, *this);
	std::thread readThread(&ReadHandler::run, &readHandler);
	std::cout << "read 스레드 성공" << std::endl;

	// write 스레드
	WriteHandler writeHandler(mSocketFd, *this);
	std::thread writeThread(&WriteHandler::run, &writeHandler);
	std::cout << "write 스레드 성공" << std::endl;

	readThread.join();
	writeThread.join();
}

void Client::close()
{
	if (mbClosed)
	{
		return;
	}

	mbClosed = true;
	std::cout << "클라이언트 연결 종료" << std::endl;

	// 소켓 닫기
	::close(mSocketFd);
}
