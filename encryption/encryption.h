#pragma once
#include <openssl/aes.h>
#include <openssl/rand.h>

void handleErrors() {
    fprintf(stderr, "Error occurred.\n");
    exit(EXIT_FAILURE);
}

void encryptData(const unsigned char* data, size_t dataLength, const char* key, unsigned char* iv, unsigned char** encryptedData, size_t* encryptedLength) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();

    if (ctx == NULL) {
        handleErrors();
    }

    if (EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, (const unsigned char*)key, iv) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        handleErrors();
    }

    *encryptedData = (unsigned char*)malloc(dataLength + AES_BLOCK_SIZE);  // Allocate memory for encrypted data
    if (*encryptedData == NULL) {
        EVP_CIPHER_CTX_free(ctx);
        handleErrors();
    }

    int len = 0;

    if (EVP_EncryptUpdate(ctx, *encryptedData, &len, data, dataLength) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        free(*encryptedData);  // Free memory before returning
        handleErrors();
    }

    *encryptedLength = len;

    if (EVP_EncryptFinal_ex(ctx, *encryptedData + len, &len) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        free(*encryptedData);  // Free memory before returning
        handleErrors();
    }

    *encryptedLength += len;

    EVP_CIPHER_CTX_free(ctx);
}


void decryptData(const unsigned char* encryptedData, size_t encryptedLength, const char* key, const unsigned char* iv, unsigned char** decryptedData, size_t* decryptedLength) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();

    if (ctx == NULL) {
        handleErrors();
    }

    if (EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, (const unsigned char*)key, iv) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        handleErrors();
    }

    *decryptedData = (unsigned char*)malloc(encryptedLength);  // Allocate memory for decrypted data
    if (*decryptedData == NULL) {
        EVP_CIPHER_CTX_free(ctx);
        handleErrors();
    }

    int len = 0;

    if (EVP_DecryptUpdate(ctx, *decryptedData, &len, encryptedData, encryptedLength) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        free(*decryptedData);  // Free memory before returning
        handleErrors();
    }

    *decryptedLength = len;

    if (EVP_DecryptFinal_ex(ctx, *decryptedData + len, &len) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        free(*decryptedData);  // Free memory before returning
        handleErrors();
    }

    *decryptedLength += len;

    EVP_CIPHER_CTX_free(ctx);
}