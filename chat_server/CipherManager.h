#ifndef CIPHER_MANAGER_H
#define CIPHER_MANAGER_H

#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>

class CipherManager {

private:

public:
    static void handleErrors();

    //RSA 키 생성
    static EVP_PKEY* create_RSA_keypair();
    
    //RSA 키 저장
    static void save_keys(EVP_PKEY* keypair);

    // 복호화
    static void aes_decrypt(const unsigned char* ciphertext, int ciphertext_len, const unsigned char* key, const unsigned char* iv, unsigned char* plaintext); 
    static void rsa_decrypt(EVP_PKEY* key, const unsigned char* encrypted, size_t encrypted_len, unsigned char* decrypted, size_t& decrypted_len);             

    // 암호화
    static void aes_encrypt(const unsigned char* plaintext, const unsigned char* key, const unsigned char* iv, unsigned char* ciphertext, int& ciphertext_len);
    static void rsa_encrypt(EVP_PKEY* key, const unsigned char* message, unsigned char* encrypted, size_t& encrypted_len);


};

#endif // CIPHER_MANAGER_H