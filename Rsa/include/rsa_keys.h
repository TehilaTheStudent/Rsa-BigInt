#ifndef __RSA_KEYS_H__
#define __RSA_KEYS_H__

#include "big_int64.h"

#include "asymmetric_keys.h"

class RsaKeys : public AsymmetricKeys {
   public:
    BigInt64 pubKey;
    BigInt64 priKey;
    BigInt64 n;
    size_t keySize;

    // Default constructor
    RsaKeys();

    // Parameterized constructor
    RsaKeys(const BigInt64 &pubKey, const BigInt64 &priKey,
            const BigInt64 &n,size_t keySize);

    // Copy constructor
    RsaKeys(const RsaKeys &other);

    // Destructor
    ~RsaKeys();

    // Copy assignment operator
    RsaKeys &operator=(const RsaKeys &other);
};

#endif  // __RSA_KEYS_H__