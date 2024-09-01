#ifndef __RSA_H__
#define __RSA_H__

#include "big_int64.h"
#include "rsa_keys.h"

// Function to generate a random prime number of a specified bit length
void rsaGeneratePrime(size_t bits, BigInt64 &prime);

void rsaKeysGeneration(const BigInt64 &p, const BigInt64 &q, size_t keySize,
                       BigInt64 &e, BigInt64 &d);

RsaKeys *rsaGenerateKeys(size_t keySize);

// Function to pad plaintext to fit RSA encryption
void rsaPkcs1v15Pad(const uint8_t *plaintext, size_t plaintextLen,
                    uint8_t *padded, size_t keySize) ;

// Function to encrypt data using RSA with padding

void rsaEncrypt(const uint8_t *plaintext, size_t plaintextLen,
                const BigInt64 &n, const BigInt64 &key, uint8_t *ciphertext,
                size_t ciphertextLen, size_t keySize);

// Function to remove padding from decrypted message
void rsaPkcs1v15Unpad(const uint8_t *padded, size_t paddedLen,
                      uint8_t *plaintext, size_t *plaintextLen);

// Function to decrypt data using RSA with padding
void rsaDecrypt(const uint8_t *ciphertext, size_t ciphertextLen,
                const BigInt64 &n, const BigInt64 &key, uint8_t *plaintext,
                size_t *plaintextLen, size_t keySize);

size_t rsaGetEncryptedLen(size_t keySize);

size_t rsaGetDecryptedLen(size_t keySize);

#endif // __RSA_H__