#include <gtest/gtest.h>

#include "asymmetric_keys.h"
#include "rsa.h"
#include "rsa_keys.h"

#define KEY_SIZE 1024
// Test for rsa_encrypt and rsa_decrypt functions
TEST(RSATest, EncryptDecrypt) {
  for (int i = 0; i < 1000; i++) {
    // std::cout << i << "------------" << std::endl;
    BigInt64 n =
        "1384799582626321342570643223726619489022593716164676201538324916125316"
        "3530294731968323415427792817749851984068911598015657119398286805957288"
        "5272195319718078116374649597519158731938081360618058003826595676876687"
        "4183301990366951787419574778344978193559435918053730630525370182838047"
        "94563712646823358120276366211";
    BigInt64 e = "65537";
    BigInt64 d =
        "8383475880725673294565222216674484927222348837334381079633154659323692"
        "7647462826269499387870659734770802540730734969355697535013240647498964"
        "6780355045330238180169084780938627858915414929367442819695585963508598"
        "2080583966391952641993405398689112636999156420560573899214685697990344"
        "4245612304000269825988560097";
    // AsymmetricKeys *asymmetricKeys = rsaGenerateKeys(KEY_SIZE);
    AsymmetricKeys *asymmetricKeys = new RsaKeys(e, d, n, KEY_SIZE);

    RsaKeys *rsaKeys = (RsaKeys *)asymmetricKeys;

    // Define data to encrypt
    const uint8_t plaintext[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    size_t plaintextLen = sizeof(plaintext);

    // Encrypt the data
    size_t ciphertextLen = rsaGetEncryptedLen(rsaKeys->keySize);
    uint8_t *ciphertext = new uint8_t[ciphertextLen];
    rsaEncrypt(plaintext, plaintextLen, rsaKeys->n, rsaKeys->pubKey, ciphertext,
               ciphertextLen, rsaKeys->keySize);

    // Decrypt the data
    size_t decrypted_len = rsaGetDecryptedLen(rsaKeys->keySize);
    uint8_t *decrypted = new uint8_t[decrypted_len];
    rsaDecrypt(ciphertext, ciphertextLen, rsaKeys->n, rsaKeys->priKey,
               decrypted, &decrypted_len, rsaKeys->keySize);

    // Check that decrypted data matches the original plaintext
    EXPECT_EQ(plaintextLen, decrypted_len);
    EXPECT_EQ(memcmp(plaintext, decrypted, plaintextLen), 0);

    // Clean up
    delete[] ciphertext;
    delete[] decrypted;
  }
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}