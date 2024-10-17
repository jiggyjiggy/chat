#include <iostream>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 12345
#define BUFFER_SIZE 1024

int main()
{
	int serverSocketFd;
	int socketFd;
	struct sockaddr_in address;
	int addrlen = sizeof(address);
	char buffer[BUFFER_SIZE] = { 0, };

	// 소켓 생성
	if ((serverSocketFd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		std::cerr << "소켓 생성 실패" << std::endl;
		return -1;
	}

	// 주소 설정
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	// 소켓 바인딩
	if (bind(serverSocketFd, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		std::cerr << "바인딩 실패" << std::endl;
		close(serverSocketFd);
		return -1;
	}

	// 서버가 클라이언트 연결을 기다림
	if (listen(serverSocketFd, 3) < 0)
	{
		std::cerr << "리스닝 실패" << std::endl;
		close(serverSocketFd);
		return -1;
	}

	std::cout << "서버가 포트 " << PORT << "에서 클라이언트 연결을 기다리는 중..." << std::endl;

	// 클라이언트 연결 수락
	if ((socketFd = accept(serverSocketFd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
	{
		std::cerr << "클라이언트 연결 실패" << std::endl;
		close(serverSocketFd);
		return -1;
	}

	std::cout << "클라이언트 연결 성공!" << std::endl;

	// 클라이언트로부터 메시지 읽기
	ssize_t bytes_read;
	while ((bytes_read = read(socketFd, buffer, BUFFER_SIZE)) > 0)
	{
		std::cout << "받은 메시지: " << std::string(buffer, bytes_read) << std::endl;

		// 클라이언트에게 받은 메시지 그대로 다시 전송 (에코)
		send(socketFd, buffer, bytes_read, 0);
	}

	if (bytes_read == 0)
	{
		std::cout << "클라이언트 연결이 종료되었습니다." << std::endl;
	}
	else
	{
		std::cerr << "읽기 실패" << std::endl;
	}

	// 소켓 종료
	close(socketFd);
	close(serverSocketFd);

	return 0;
}
