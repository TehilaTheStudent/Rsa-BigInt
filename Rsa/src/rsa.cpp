#include "rsa.h"

#include <cstring>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <stdexcept>

using namespace std;
void printBufferHexa(const unsigned char *buffer, size_t len) {
  for (size_t i = 0; i < len; ++i) {
    std::cout << std::hex << std::setw(2) << std::setfill('0')
              << static_cast<int>(buffer[i]) << " ";

    // Print a new line every 16 bytes for better readability
    if ((i + 1) % 16 == 0) {
      std::cout << std::endl;
    }
  }
  std::cout << std::endl;
  // Reset the stream format back to decimal
  std::cout << std::dec;
}

void rsaGeneratePrime(size_t bits, BigInt64 &prime) {

  // Generate a random number with exactly 'bits' bits
  BigInt64 random = BigInt64(BigInt64::CreateModes::RANDOM, bits);

  // Find the next prime number greater than or equal to 'prime'
  prime = random.nextPrime();
}

void rsaKeysGeneration(const BigInt64 &p, const BigInt64 &q, size_t keySize,
                       BigInt64 &e, BigInt64 &d) {
  BigInt64 phi;
  BigInt64 gcdVal;

  // phi(n) = (p-1) * (q-1)
  BigInt64 pMinus1 = p - 1;
  BigInt64 qMinus1 = q - 1;
  phi = pMinus1 * qMinus1;

  // Choose e such that 1 < e < phi and gcd(e, phi) = 1 (coprime)
  // A common choice for e is 65537
  e = 65537;
  gcdVal = gcd(e, phi);
  while (gcdVal != 1) {
    e += 2;
    if (e > phi)
      e = 17;
  }

  // d is the modular inverse of e modulo ϕ(n) or e^-1 mod phi
  //  0<d<phi and  (d * e) % phi = 1 ,<- (d*e -1) is divisible by phi
  d = modularInverse(e, phi);
}

RsaKeys *rsaGenerateKeys(size_t keySize) {

  // Generate prime numbers for public and private keys
  BigInt64 p, q, n, e, d;
  // do {
  // Generate large prime numbers
  rsaGeneratePrime(keySize / 2, p);
  rsaGeneratePrime(keySize / 2, q);

  // Compute n = p * q
  n = p * q;
  // } while (n.bitsCount()!= keySize);

  rsaKeysGeneration(p, q, keySize, e, d);

  // Now store in HSM the e, d, n for keyId
  RsaKeys *rsaKeys = new RsaKeys(e, d, n, keySize);
  return rsaKeys;
}

void rsaPkcs1v15Pad(const uint8_t *plaintext, size_t plaintextLen,
                    uint8_t *padded, size_t keySize) {
  size_t paddingLen = keySize - plaintextLen - 3; // Padding length
  if (paddingLen < 8) {                           // Minimum padding length
    throw std::runtime_error("Plaintext is too long for padding");
  }

  // Start with 0x01 0x02
  padded[0] = 0x01;
  padded[1] = 0x02;

  // Add non-zero random padding
  for (size_t i = 2; i < paddingLen + 2; i++) {
    uint8_t rnd = static_cast<uint8_t>(rand() % 256);
    while (rnd == 0) {
      rnd = static_cast<uint8_t>(rand() % 256);
    }
    padded[i] = rnd;
  }

  // Add 0x00 separator
  padded[paddingLen + 2] = 0x00;

  // Copy the plaintext
  std::memcpy(padded + paddingLen + 3, plaintext, plaintextLen);
}

void rsaEncrypt(const uint8_t *plaintext, size_t plaintextLen,
                const BigInt64 &n, const BigInt64 &key, uint8_t *ciphertext,
                size_t ciphertextLen, size_t keySize) {
  // Get key size in bytes
  size_t keySizeBytes = keySize / 8;

  if (ciphertextLen != keySizeBytes) {
    throw std::runtime_error("Invalid ciphertext length");
  }
  size_t paddedLen = keySizeBytes;
  printBufferHexa(plaintext, plaintextLen);
  // Padding plaintext to keySizeBytes
  uint8_t *padded = new uint8_t[keySizeBytes];
  rsaPkcs1v15Pad(plaintext, plaintextLen, padded, keySizeBytes);

  printBufferHexa(padded, paddedLen);
  // Convert padded plaintext to BigInt64
  BigInt64 plainNumber(padded, paddedLen);
  cout << plainNumber << endl;
  cout<<key<<endl;
  cout<<n<<endl;
  // Encrypt message:
  BigInt64 cipherNumber = modularExponentiation(plainNumber, key, n);
  cout << cipherNumber << endl;
  memset(ciphertext, 0, keySizeBytes);
  // Convert ciphertext from mpz_class to uint8_t

  //   size_t offset = keySizeBytes - (mpz_sizeinbase(c.get_mpz_t(), 2) + 7) /
  //   8;

  cipherNumber.exportTo(ciphertext, ciphertextLen);
  printBufferHexa(ciphertext, ciphertextLen);
  //   mpz_export(ciphertext + offset, nullptr, 1, sizeof(ciphertext[0]), 0, 0,
  //              c.get_mpz_t());

  delete[] padded;
}

void rsaDecrypt(const uint8_t *ciphertext, size_t ciphertextLen,
                const BigInt64 &n, const BigInt64 &key, uint8_t *plaintext,
                size_t *plaintextLen, size_t keySize) {
  size_t keySizeBytes = keySize / 8;

  if (ciphertextLen != keySizeBytes) {
    throw std::runtime_error("Invalid ciphertext length");
  }

  //   mpz_class c, m;
  printBufferHexa(ciphertext, ciphertextLen);
  // Convert ciphertext to BigInt64
  BigInt64 cipherNumber(ciphertext, ciphertextLen);
  cout << cipherNumber << endl;
  //   mpz_import(c.get_mpz_t(), keySizeBytes, 1, sizeof(ciphertext[0]), 0, 0,
  //              ciphertext);

  // Decrypt message: m = c^d % n
  //   mpz_powm(m.get_mpz_t(), c.get_mpz_t(), key.get_mpz_t(), n.get_mpz_t());
  BigInt64 plainNumber = modularExponentiation(cipherNumber, key, n);
  cout << plainNumber << endl;
  // Convert decrypted message from mpz_class to uint8_t
  uint8_t *padded = new uint8_t[keySizeBytes];
  size_t paddedLen = keySizeBytes;
  // mpz_export(padded, nullptr, 1, sizeof(padded[0]), 0, 0, m.get_mpz_t());
  plainNumber.exportTo(padded, paddedLen);
  printBufferHexa(padded, paddedLen);

  // Remove padding
  rsaPkcs1v15Unpad(padded, paddedLen, plaintext, plaintextLen);
  printBufferHexa(plaintext, *plaintextLen);
  delete[] padded;
}

size_t rsaGetEncryptedLen(size_t keySize) { return keySize / 8; }

size_t rsaGetDecryptedLen(size_t keySize) {
  // Minimum padding length for PKCS#1 v1.5 is 11 bytes
  const size_t minPaddingLength = 11;

  // Remove the padding: The maximum length of the plaintext is keySize -
  // minPaddingLength
  return keySize / 8 - minPaddingLength;
}

void rsaPkcs1v15Unpad(const uint8_t *padded, size_t paddedLen,
                      uint8_t *plaintext, size_t *plaintextLen) {
  if (paddedLen < 11 || padded[0] != 0x01 || padded[1] != 0x02) {
    throw std::runtime_error("Invalid padding");
  }

  // Find 0x00 separator
  size_t i = 2;
  while (i < paddedLen && padded[i] != 0x00) {
    ++i;
  }

  if (i == paddedLen) {
    throw std::runtime_error("Invalid padding: No separator found");
  }

  // Copy plaintext
  *plaintextLen = paddedLen - i - 1;
  std::memcpy(plaintext, padded + i + 1, *plaintextLen);
}