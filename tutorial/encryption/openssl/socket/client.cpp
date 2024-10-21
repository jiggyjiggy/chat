#include <iostream>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

void handleErrors() {
    ERR_print_errors_fp(stderr);
    abort();
}

// AES 암호화 함수
void aes_encrypt(const unsigned char* plaintext, const unsigned char* key, const unsigned char* iv, unsigned char* ciphertext, int& ciphertext_len) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) handleErrors();

    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv)) handleErrors();

    int len;
    if (1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, strlen((const char*)plaintext))) handleErrors();
    ciphertext_len = len;

    if (1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len)) handleErrors();
    ciphertext_len += len;

    EVP_CIPHER_CTX_free(ctx);
}

// RSA 암호화 함수
void rsa_encrypt(EVP_PKEY* key, const unsigned char* message, unsigned char* encrypted, size_t& encrypted_len) {
    EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new(key, NULL);
    if (!ctx) handleErrors();

    if (EVP_PKEY_encrypt_init(ctx) <= 0) handleErrors();

    if (EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_OAEP_PADDING) <= 0) handleErrors();

    // 암호화할 메시지 길이 계산
    if (EVP_PKEY_encrypt(ctx, NULL, &encrypted_len, message, strlen((const char*)message)) <= 0) handleErrors();

    // 메시지 암호화
    if (EVP_PKEY_encrypt(ctx, encrypted, &encrypted_len, message, strlen((const char*)message)) <= 0) handleErrors();

    EVP_PKEY_CTX_free(ctx);
}

int main() {
    const char* server_ip = "127.0.0.1";
    int server_port = 12345;
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    inet_pton(AF_INET, server_ip, &server_addr.sin_addr);
    
    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Connection failed!" << std::endl;
        return -1;
    }
    
    // RSA 공개 키 불러오기
    FILE* pub_file = fopen("server_public.pem", "r");
    EVP_PKEY* server_pub_key = PEM_read_PUBKEY(pub_file, NULL, NULL, NULL);
    fclose(pub_file);
    
    // AES 키 및 IV 생성
    unsigned char aes_key[16], aes_iv[16];
    RAND_bytes(aes_key, sizeof(aes_key));
    RAND_bytes(aes_iv, sizeof(aes_iv));
    
    // AES 키와 IV를 서버로 RSA로 암호화하여 전송
    unsigned char encrypted_key[256];
    size_t encrypted_key_len;
    rsa_encrypt(server_pub_key, aes_key, encrypted_key, encrypted_key_len);
    send(sock, encrypted_key, encrypted_key_len, 0);
    
    send(sock, aes_iv, sizeof(aes_iv), 0);
    
    // 암호화된 메시지 전송
    const char* plaintext = "Hello, secure world!";
    unsigned char ciphertext[128];
    int ciphertext_len;
    aes_encrypt((unsigned char*)plaintext, aes_key, aes_iv, ciphertext, ciphertext_len);
    
    send(sock, ciphertext, ciphertext_len, 0);
    
    EVP_PKEY_free(server_pub_key);
    close(sock);
    
    return 0;
}
