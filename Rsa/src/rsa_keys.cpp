#include "rsa_keys.h"

// Default constructor
RsaKeys::RsaKeys() : pubKey(0), priKey(0), n(0),keySize(0) {}

// Parameterized constructor
RsaKeys::RsaKeys(const BigInt64 &pubKey, const BigInt64 &priKey, const BigInt64 &n,size_t keySize)
    : pubKey(pubKey), priKey(priKey), n(n),keySize(keySize) {}

// Copy constructor
RsaKeys::RsaKeys(const RsaKeys &other) : pubKey(other.pubKey), priKey(other.priKey), n(other.n),keySize(other.keySize) {}

// Destructor
RsaKeys::~RsaKeys() {}

// Copy assignment operator
RsaKeys &RsaKeys::operator=(const RsaKeys &other) {
    if (this != &other) {
        pubKey = other.pubKey;
        priKey = other.priKey;
        n = other.n;
        keySize = other.keySize;
    }
    return *this;
}