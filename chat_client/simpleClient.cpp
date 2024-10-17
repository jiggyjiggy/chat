#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_IP "127.0.0.1" // 서버 IP 주소
#define SERVER_PORT 12345	  // 서버 포트 번호
#define BUFFER_SIZE 1024	  // 버퍼 크기

int main()
{
	int sock = 0;
	struct sockaddr_in serv_addr;
	char buffer[BUFFER_SIZE] = { 0, };

	// 소켓 생성
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		std::cerr << "소켓 생성 실패" << std::endl;
		return -1;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(SERVER_PORT);

	// IP 주소 변환
	if (inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr) <= 0)
	{
		std::cerr << "유효하지 않은 주소 또는 주소 변환 실패" << std::endl;
		return -1;
	}

	// 서버에 연결
	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		std::cerr << "서버 연결 실패" << std::endl;
		return -1;
	}

	// 서버로 메시지 전송
	const char *message = "Hello, Server!";
	send(sock, message, strlen(message), 0);
	std::cout << "메시지 전송: " << message << std::endl;

	// 서버로부터 응답 받기
	int bytesRead = read(sock, buffer, BUFFER_SIZE);
	if (bytesRead > 0)
	{
		std::cout << "서버 응답: " << buffer << std::endl;
	}
	else
	{
		std::cerr << "서버로부터 응답 받기 실패" << std::endl;
	}

	// 소켓 닫기
	close(sock);

	return 0;
}
