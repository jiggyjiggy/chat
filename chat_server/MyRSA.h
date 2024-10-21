#ifndef RSA_H
#define RSA_H

#include <openssl/evp.h>
#include <openssl/pem.h>
#include <string>

class MyRSA {
public:
    MyRSA(const std::string& public_key_file);
    ~MyRSA();

    // 암호화 및 복호화
    void encrypt(const unsigned char* plaintext, unsigned char* encrypted, size_t& encrypted_len);
    void decrypt(const unsigned char* encrypted, size_t encrypted_len, unsigned char* decrypted, size_t& decrypted_len);

    // RSA 키페어 생성 및 저장
	static EVP_PKEY* create_RSA_keypair();
	static void save_keys(EVP_PKEY* keypair);

private:
    EVP_PKEY* mRsaKey;
    static void handleErrors();
};

#endif // RSA_H
