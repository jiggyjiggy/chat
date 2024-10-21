#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int server_fd, client_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    // 1. 소켓 생성
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("소켓 실패");
        exit(EXIT_FAILURE);
    }

    // 2. 소켓 옵션 설정
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt 실패");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // 3. 서버 주소 설정
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // 4. bind
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind 실패");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // 5. listen
    if (listen(server_fd, 3) < 0) {
        perror("listen 실패");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    std::cout << "서버 대기 중..." << std::endl;

    // 6. 클라이언트 연결 수락
    if ((client_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
        perror("accept 실패");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    std::cout << "클라이언트 연결 수락됨" << std::endl;

    // 7. 클라이언트로부터 데이터 수신
    int valread = read(client_socket, buffer, BUFFER_SIZE);
    std::cout << "클라이언트로부터 받은 메시지: " << buffer << std::endl;

    // 8. 데이터 처리 후 연결 종료 (4-way handshake)
    std::cout << "서버: 연결 종료 시작 (4-way handshake)" << std::endl;

    // 쓰기 방향 종료
    shutdown(client_socket, SHUT_WR);
    std::cout << "서버: 쓰기 종료" << std::endl;

    // 클라이언트가 FIN을 보낼 때까지 대기
    valread = read(client_socket, buffer, BUFFER_SIZE);
    std::cout << "클라이언트 FIN 확인" << std::endl;

    // 소켓 완전 종료
    close(client_socket);
    std::cout << "서버: 소켓 종료" << std::endl;

    close(server_fd); // 서버 프로세스의 소켓
    return 0;
}
