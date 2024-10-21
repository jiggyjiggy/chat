#include "CipherManager.h"
#include <iostream>
#include <cstring>

void CipherManager::handleErrors() {
    ERR_print_errors_fp(stderr);
    abort();
}

// AES 키 생성
EVP_PKEY* CipherManager::create_RSA_keypair() {
    EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL);
    if (!ctx) handleErrors();

    if (EVP_PKEY_keygen_init(ctx) <= 0) handleErrors();
    
    if (EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, 2048) <= 0) handleErrors();

    EVP_PKEY* keypair = NULL;
    if (EVP_PKEY_keygen(ctx, &keypair) <= 0) handleErrors();

    EVP_PKEY_CTX_free(ctx);
    return keypair;
}

// AES 키 저장
void CipherManager::save_keys(EVP_PKEY* keypair) {
    // Save private key
    FILE* priv_file = fopen("private.pem", "w");
    PEM_write_PrivateKey(priv_file, keypair, NULL, NULL, 0, NULL, NULL);
    fclose(priv_file);

    // Save public key
    FILE* pub_file = fopen("public.pem", "w");
    PEM_write_PUBKEY(pub_file, keypair);
    fclose(pub_file);
}

// AES 복호화 함수
void CipherManager::aes_decrypt(const unsigned char* ciphertext, int ciphertext_len, const unsigned char* key, const unsigned char* iv, unsigned char* plaintext) {
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
void CipherManager::rsa_decrypt(EVP_PKEY* key, const unsigned char* encrypted, size_t encrypted_len, unsigned char* decrypted, size_t& decrypted_len) {
    EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new(key, NULL);
    if (!ctx) handleErrors();

    if (EVP_PKEY_decrypt_init(ctx) <= 0) handleErrors();

    if (EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_OAEP_PADDING) <= 0) handleErrors();

    if (EVP_PKEY_decrypt(ctx, NULL, &decrypted_len, encrypted, encrypted_len) <= 0) handleErrors();

    if (EVP_PKEY_decrypt(ctx, decrypted, &decrypted_len, encrypted, encrypted_len) <= 0) handleErrors();

    EVP_PKEY_CTX_free(ctx);
}

// AES 암호화 함수
void CipherManager::aes_encrypt(const unsigned char* plaintext, const unsigned char* key, const unsigned char* iv, unsigned char* ciphertext, int& ciphertext_len) {
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
void CipherManager::rsa_encrypt(EVP_PKEY* key, const unsigned char* message, unsigned char* encrypted, size_t& encrypted_len) {
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

