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

// AES 복호화 함수
void aes_decrypt(const unsigned char* ciphertext, int ciphertext_len, const unsigned char* key, const unsigned char* iv, unsigned char* plaintext) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) handleErrors();

    if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv)) handleErrors();

    int len;
    if (1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len)) handleErrors();

    int plaintext_len = len;
    if (1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len)) handleErrors();
    plaintext_len += len;

    plaintext[plaintext_len] = '\0';  // NULL 종료

    EVP_CIPHER_CTX_free(ctx);
}

// RSA 복호화 함수
void rsa_decrypt(EVP_PKEY* key, const unsigned char* encrypted, size_t encrypted_len, unsigned char* decrypted, size_t& decrypted_len) {
    EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new(key, NULL);
    if (!ctx) handleErrors();

    if (EVP_PKEY_decrypt_init(ctx) <= 0) handleErrors();

    if (EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_OAEP_PADDING) <= 0) handleErrors();

    if (EVP_PKEY_decrypt(ctx, NULL, &decrypted_len, encrypted, encrypted_len) <= 0) handleErrors();

    if (EVP_PKEY_decrypt(ctx, decrypted, &decrypted_len, encrypted, encrypted_len) <= 0) handleErrors();

    EVP_PKEY_CTX_free(ctx);
}

int main() {
    int server_port = 12345;
    int server_sock = socket(AF_INET, SOCK_STREAM, 0);
    
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    
    if (bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Binding failed!" << std::endl;
        return -1;
    }
    
    listen(server_sock, 3);
    
    int client_sock = accept(server_sock, NULL, NULL);
    if (client_sock < 0) {
        std::cerr << "Accept failed!" << std::endl;
        return -1;
    }
    
    // RSA 개인 키 불러오기
    FILE* priv_file = fopen("server_private.pem", "r");
    EVP_PKEY* server_priv_key = PEM_read_PrivateKey(priv_file, NULL, NULL, NULL);
    fclose(priv_file);
    
    // AES 키 복호화
    unsigned char encrypted_key[256];
    recv(client_sock, encrypted_key, sizeof(encrypted_key), 0);
    
    unsigned char aes_key[16];
    size_t decrypted_key_len;
    rsa_decrypt(server_priv_key, encrypted_key, sizeof(encrypted_key), aes_key, decrypted_key_len);
    
    // IV 수신
    unsigned char aes_iv[16];
    recv(client_sock, aes_iv, sizeof(aes_iv), 0);
    
    // 메시지 수신 및 복호화
    unsigned char ciphertext[128];
    int ciphertext_len = recv(client_sock, ciphertext, sizeof(ciphertext), 0);
    
    unsigned char decryptedtext[128];
    aes_decrypt(ciphertext, ciphertext_len, aes_key, aes_iv, decryptedtext);
    
    std::cout << "Decrypted message: " << decryptedtext << std::endl;
    
    EVP_PKEY_free(server_priv_key);
    close(client_sock);
    close(server_sock);
    
    return 0;
}
