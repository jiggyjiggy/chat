#include <arpa/inet.h>
#include <iostream>
#include <unistd.h>

/*

// Socket address, internet style.

struct sockaddr_in {
	__uint8_t       sin_len;
	sa_family_t     sin_family;		// 주소체계(PF_INET)
	in_port_t       sin_port;		// 포트번호
	struct  in_addr sin_addr;		// IP 주소(32비트)
	char            sin_zero[8];	// 16바이트로 만들기 위한 패딩
};

*/
/*

// Internet address (a structure for historical reasons)

struct in_addr {
	in_addr_t s_addr;
};

*/

int main()
{
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;

	const int port = 12321;
	const int queue = 10;
	const int cl_size = sizeof(client_addr);
	char buffer[4096];

	server_addr.sin_family = PF_INET; // https://www.bangseongbeom.com/af-inet-vs-pf-inet.html ; 확장을 위해 두가지 체계를 준비했으나 사용하지 않음, AF_INET을 보통 씀
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	int server_fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (bind(server_fd, (sockaddr *)&server_addr, sizeof(server_addr)) == -1)
	{
		std::cerr << "bind() error" << std::endl;
	}
	else
	{
		std::cout << "binding" << std::endl;
	}

	if (listen(server_fd, queue) == -1)
	{
		std::cout << "listen() error" << std::endl;
	}
	else
	{
		std::cout << "listening port " << port << std::endl;
	}

	int client_fd = accept(server_fd, (sockaddr *)&client_addr, (socklen_t *)&cl_size);
	if (client_fd == -1)
	{
		std::cout << "accept error()" << std::endl;
	}
	else
	{
		int n = read(client_fd, buffer, 4096);

		write(client_fd, buffer, n);
	}

	close(client_fd);
	close(server_fd);

	return 0;
}