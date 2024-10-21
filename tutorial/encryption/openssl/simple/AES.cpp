#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/err.h>
#include <iostream>
#include <cstring>

void handleErrors() {
    ERR_print_errors_fp(stderr);
    abort();
}

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

void aes_decrypt(const unsigned char* ciphertext, int ciphertext_len, const unsigned char* key, const unsigned char* iv, unsigned char* plaintext) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) handleErrors();

    if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv)) handleErrors();

    int len;
    if (1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len)) handleErrors();

    int plaintext_len = len;
    if (1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len)) handleErrors();
    plaintext_len += len;

    plaintext[plaintext_len] = '\0';  // Add NULL terminator to make it a proper C string

    EVP_CIPHER_CTX_free(ctx);
}

int main() {
    // AES 128-bit key and IV
    unsigned char key[16];
    unsigned char iv[16];
    RAND_bytes(key, sizeof(key));
    RAND_bytes(iv, sizeof(iv));

    const char* plaintext = "Hello world";
    unsigned char ciphertext[128];
    unsigned char decryptedtext[128];
    int ciphertext_len;

    std::cout << "Original message: " << plaintext << std::endl;

    // Encrypt
    aes_encrypt((unsigned char*)plaintext, key, iv, ciphertext, ciphertext_len);
    std::cout << "Encrypted message: ";
    for (int i = 0; i < ciphertext_len; i++) {
        printf("%02x", ciphertext[i]);
    }
    std::cout << std::endl;

    // Decrypt
    aes_decrypt(ciphertext, ciphertext_len, key, iv, decryptedtext);
    std::cout << "Decrypted message: " << decryptedtext << std::endl;

    return 0;
}
