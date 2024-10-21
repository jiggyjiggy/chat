#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char *hello = (char*)"클라이언트 메시지";
    char buffer[BUFFER_SIZE] = {0};

    // 1. 소켓 생성
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "소켓 생성 실패" << std::endl;
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // 2. 서버 주소 설정
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        std::cerr << "잘못된 주소/주소 변환 실패" << std::endl;
        return -1;
    }

    // 3. 서버에 연결 요청
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "연결 실패" << std::endl;
        return -1;
    }

    // 4. 서버에 데이터 전송
    send(sock, hello, strlen(hello), 0);
    std::cout << "클라이언트: 서버로 메시지 전송" << std::endl;

    // 서버로부터 FIN을 받을 때까지 대기
    int valread = read(sock, buffer, BUFFER_SIZE);
    std::cout << "서버 FIN 확인" << std::endl;

    // 쓰기 방향 종료
    shutdown(sock, SHUT_WR);
    std::cout << "클라이언트: 쓰기 종료" << std::endl;

    // 소켓 완전 종료
    close(sock);
    std::cout << "클라이언트: 소켓 종료" << std::endl;

    return 0;
}
