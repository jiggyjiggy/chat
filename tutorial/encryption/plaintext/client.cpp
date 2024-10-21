#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>

int main()
{
	char buffer[4096];
	struct sockaddr_in server_addr;
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(12321);

	int client_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	std::cout << client_fd << std::endl;

	if (connect(client_fd, (sockaddr*)&server_addr, sizeof(server_addr)) == -1)
	{
		perror("connect()");
		std::cout << "connect() error" << std::endl;
		return -1;
	}

	write(client_fd, "HELLO", 5);
	read(client_fd, buffer, 4096);

	close(client_fd);

	return 0;
}