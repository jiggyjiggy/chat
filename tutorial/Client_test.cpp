#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 12345
#define MESSAGE "ABCDEFGHIJKLMNOPQRSTUVWXYZ123456789012345678" // TCP HEADE : 20Byte + 44Byte = 64Byte
#define INTERVAL 1

int main()
{
	int sock;
	struct sockaddr_in server_addr;

	//소켓 생성
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0)
	{
		std::cerr << "Socket Creation failed" << std::endl;
		return -1;
	}

	//서버 주소 설정
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERVER_PORT);
	if(inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0)
	{
		std::cerr << "Invaild address/Address not supported" << std::endl;
		return -1;
	}

	if(connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
		std::cerr << "Conneted failed" << std::endl;
	}

		std::cerr << "Connected to server" << std::endl;

	
	while(true)
	{
		ssize_t sent_bytes = send(sock, MESSAGE, strlen(MESSAGE), 0);
		if(sent_bytes < 0)
		{
			std::cerr << "Message sent failed" << std::endl;	
		}

		std::cout << "Send Message : 64Byte" << std::endl;

		sleep(INTERVAL);
	}

	close(sock);
	return 0;
}
