#include <gtest/gtest.h>

#include "asymmetric_keys.h"
#include "rsa.h"
#include "rsa_keys.h"
#include <iostream>
using namespace std;
#define KEY_SIZE 1024
// Test for rsa_encrypt and rsa_decrypt functions
TEST(RSATest, EncryptDecrypt) {
  // for (int i = 0; i < 1000; i++) {
  //  std::cout << i << "------------" << std::endl;

  AsymmetricKeys *asymmetricKeys = rsaGenerateKeys(KEY_SIZE);

  // AsymmetricKeys *asymmetricKeys = new RsaKeys(e, d, n, KEY_SIZE);

  RsaKeys *rsaKeys = (RsaKeys *)asymmetricKeys;
  cout << "n" << endl << rsaKeys->n << endl;
  cout << "pub" << endl << rsaKeys->pubKey << endl;
  cout << "priv" << endl << rsaKeys->priKey << endl;

  // Define data to encrypt
  const uint8_t plaintext[] = {0xFF, 0xFF, 0xFF};
  size_t plaintextLen = sizeof(plaintext);

  // Encrypt the data
  size_t ciphertextLen = rsaGetEncryptedLen(rsaKeys->keySize);
  uint8_t *ciphertext = new uint8_t[ciphertextLen];
  rsaEncrypt(plaintext, plaintextLen, rsaKeys->n, rsaKeys->pubKey, ciphertext,
             ciphertextLen, rsaKeys->keySize);

  // Decrypt the data
  size_t decrypted_len = rsaGetDecryptedLen(rsaKeys->keySize);
  uint8_t *decrypted = new uint8_t[decrypted_len];
  rsaDecrypt(ciphertext, ciphertextLen, rsaKeys->n, rsaKeys->priKey, decrypted,
             &decrypted_len, rsaKeys->keySize);

  // Check that decrypted data matches the original plaintext
  EXPECT_EQ(plaintextLen, decrypted_len);
  EXPECT_EQ(memcmp(plaintext, decrypted, plaintextLen), 0);

  // Clean up
  delete[] ciphertext;
  delete[] decrypted;
  // }
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}