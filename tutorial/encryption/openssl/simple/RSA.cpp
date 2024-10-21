#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/evp.h> // Use EVP for encryption
#include <openssl/rand.h>
#include <iostream>
#include <cstring>

void handleErrors() {
    ERR_print_errors_fp(stderr);
    abort();
}

EVP_PKEY* create_RSA_keypair() {
    EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL);
    if (!ctx) handleErrors();

    if (EVP_PKEY_keygen_init(ctx) <= 0) handleErrors();
    
    if (EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, 2048) <= 0) handleErrors();

    EVP_PKEY* keypair = NULL;
    if (EVP_PKEY_keygen(ctx, &keypair) <= 0) handleErrors();

    EVP_PKEY_CTX_free(ctx);
    return keypair;
}

void save_keys(EVP_PKEY* keypair) {
    // Save private key
    FILE* priv_file = fopen("private.pem", "w");
    PEM_write_PrivateKey(priv_file, keypair, NULL, NULL, 0, NULL, NULL);
    fclose(priv_file);

    // Save public key
    FILE* pub_file = fopen("public.pem", "w");
    PEM_write_PUBKEY(pub_file, keypair);
    fclose(pub_file);
}

void rsa_encrypt(EVP_PKEY* keypair, const unsigned char* message, unsigned char* encrypted, size_t& encrypted_len) {
    EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new(keypair, NULL);
    if (!ctx) handleErrors();

    if (EVP_PKEY_encrypt_init(ctx) <= 0) handleErrors();

    if (EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_OAEP_PADDING) <= 0) handleErrors();

    // Determine buffer length for encrypted message
    if (EVP_PKEY_encrypt(ctx, NULL, &encrypted_len, message, strlen((const char*)message)) <= 0) handleErrors();

    // Encrypt the message
    if (EVP_PKEY_encrypt(ctx, encrypted, &encrypted_len, message, strlen((const char*)message)) <= 0) handleErrors();

    EVP_PKEY_CTX_free(ctx);
}

void rsa_decrypt(EVP_PKEY* keypair, const unsigned char* encrypted, size_t encrypted_len, unsigned char* decrypted, size_t& decrypted_len) {
    EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new(keypair, NULL);
    if (!ctx) handleErrors();

    if (EVP_PKEY_decrypt_init(ctx) <= 0) handleErrors();

    if (EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_OAEP_PADDING) <= 0) handleErrors();

    // Determine buffer length for decrypted message
    if (EVP_PKEY_decrypt(ctx, NULL, &decrypted_len, encrypted, encrypted_len) <= 0) handleErrors();

    // Decrypt the message
    if (EVP_PKEY_decrypt(ctx, decrypted, &decrypted_len, encrypted, encrypted_len) <= 0) handleErrors();

    EVP_PKEY_CTX_free(ctx);
}

int main() {
    const unsigned char* message = (unsigned char*)"Hello world";
    unsigned char encrypted[256];
    unsigned char decrypted[256];
    size_t encrypted_len = 0, decrypted_len = 0;

    EVP_PKEY* keypair = create_RSA_keypair();

    // Save keys to files
    save_keys(keypair);

    std::cout << "Original message: " << message << std::endl;

    // Encrypt
    rsa_encrypt(keypair, message, encrypted, encrypted_len);
    std::cout << "Encrypted message: ";
    for (size_t i = 0; i < encrypted_len; i++) {
        printf("%02x", encrypted[i]);
    }
    std::cout << std::endl;

    // Decrypt
    rsa_decrypt(keypair, encrypted, encrypted_len, decrypted, decrypted_len);
    decrypted[decrypted_len] = '\0';  // Null-terminate the decrypted string
    std::cout << "Decrypted message: " << decrypted << std::endl;

    EVP_PKEY_free(keypair);  // Use EVP_PKEY_free instead of RSA_free

    return 0;
}
