#include "MyRSA.h"
#include <openssl/err.h>
#include <stdexcept>

MyRSA::MyRSA(const std::string& public_key_file) {
    FILE* pub_file = fopen(public_key_file.c_str(), "r");
    if (!pub_file) {
        throw std::runtime_error("Unable to open public key file");
    }
    mRsaKey = PEM_read_PUBKEY(pub_file, NULL, NULL, NULL);
    fclose(pub_file);
    if (!mRsaKey) {
        throw std::runtime_error("Unable to read public key");
    }
}

MyRSA::~MyRSA() {
    EVP_PKEY_free(mRsaKey);
}

void MyRSA::handleErrors() {
    ERR_print_errors_fp(stderr);
    abort();
}

void MyRSA::encrypt(const unsigned char* plaintext, unsigned char* encrypted, size_t& encrypted_len) {
    EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new(mRsaKey, NULL);
    if (!ctx) handleErrors();

    if (EVP_PKEY_encrypt_init(ctx) <= 0) handleErrors();
    if (EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_OAEP_PADDING) <= 0) handleErrors();

    // Calculate buffer length
    if (EVP_PKEY_encrypt(ctx, NULL, &encrypted_len, plaintext, strlen((const char*)plaintext)) <= 0) handleErrors();

    // Encrypt the message
    if (EVP_PKEY_encrypt(ctx, encrypted, &encrypted_len, plaintext, strlen((const char*)plaintext)) <= 0) handleErrors();

    EVP_PKEY_CTX_free(ctx);
}

void MyRSA::decrypt(const unsigned char* encrypted, size_t encrypted_len, unsigned char* decrypted, size_t& decrypted_len) {
    EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new(mRsaKey, NULL);
    if (!ctx) handleErrors();

    if (EVP_PKEY_decrypt_init(ctx) <= 0) handleErrors();
    if (EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_OAEP_PADDING) <= 0) handleErrors();

    if (EVP_PKEY_decrypt(ctx, NULL, &decrypted_len, encrypted, encrypted_len) <= 0) handleErrors();
    if (EVP_PKEY_decrypt(ctx, decrypted, &decrypted_len, encrypted, encrypted_len) <= 0) handleErrors();

    EVP_PKEY_CTX_free(ctx);
}

EVP_PKEY* MyRSA::create_RSA_keypair() {
	EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL);
	if (!ctx) handleErrors();

	if (EVP_PKEY_keygen_init(ctx) <= 0) handleErrors();
	
	if (EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, 2048) <= 0) handleErrors();

	EVP_PKEY* keypair = NULL;
	if (EVP_PKEY_keygen(ctx, &keypair) <= 0) handleErrors();

	EVP_PKEY_CTX_free(ctx);
	return keypair;
}

void MyRSA::save_keys(EVP_PKEY* keypair) {
	// Save private key
	FILE* priv_file = fopen("private.pem", "w");
	PEM_write_PrivateKey(priv_file, keypair, NULL, NULL, 0, NULL, NULL);
	fclose(priv_file);

	// Save public key
	FILE* pub_file = fopen("public.pem", "w");
	PEM_write_PUBKEY(pub_file, keypair);
	fclose(pub_file);
}
