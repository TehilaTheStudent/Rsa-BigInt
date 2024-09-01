#ifndef __BIG_INT_64__
#define __BIG_INT_64__

#include "big_int10.h"
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

class BigInt64 {
public:
  // ctors
  BigInt64(const std::string &str);
  BigInt64(long long val = 0);
  BigInt64(uint64_t uval);
  BigInt64(int val);
  BigInt64(bool isNegative);
  BigInt64(const char *str);
  BigInt64(const uint8_t *str, size_t strLen);
  BigInt64(std::vector<uint64_t> v, bool isNegative);
  enum CreateModes { RANDOM };
  BigInt64(CreateModes mode, int bits);
  BigInt64(CreateModes mode, uint64_t min, const BigInt64 &max);

  // operators
  // comparison
  bool operator==(const BigInt64 &b) const;
  bool operator!=(const BigInt64 &b) const;
  bool operator<(const BigInt64 &b) const;
  bool operator>(const BigInt64 &b) const;
  bool operator<=(const BigInt64 &b) const;
  bool operator>=(const BigInt64 &b) const;
  // arithematic operations
  // addition
  BigInt64 &operator++();
  BigInt64 operator++(int);
  BigInt64 &operator--();
  BigInt64 operator--(int);

  BigInt64 &operator+=(const BigInt64 &b);
  BigInt64 operator+(const BigInt64 &b) const;
  // subtruction
  BigInt64 &operator-=(const BigInt64 &b);
  BigInt64 operator-(const BigInt64 &b) const;
  // multiplication
  BigInt64 &operator*=(const BigInt64 &b);
  BigInt64 operator*(const BigInt64 &b) const;
  // division
  BigInt64 &operator/=(const BigInt64 &b);
  BigInt64 operator/(const BigInt64 &b) const;
  // modulus
  BigInt64 &operator%=(const BigInt64 &b);
  BigInt64 operator%(const BigInt64 &b) const;
  // power
  BigInt64 &operator^=(const BigInt64 &b);
  BigInt64 operator^(const BigInt64 &b) const;
  // right shift - division
  BigInt64 &operator>>=(uint64_t n);
  BigInt64 operator>>(uint64_t n) const;
  // left shift - multiplication
  BigInt64 &operator<<=(uint64_t n);
  BigInt64 operator<<(uint64_t n) const;
  //<<
  friend std::ostream &operator<<(std::ostream &out, const BigInt64 &a);
  // basic utils
  bool isZero() const { return limbsCount() == 1 && limbs[0] == 0; }
  int limbsCount() const { return limbs.size(); }
  int bitsCount() const;
  bool isEven() const { return (limbs[0] & 1) == 0; }
  // extras
  friend BigInt64 modularExponentiation(BigInt64 base, BigInt64 exponent,
                                        const BigInt64 &modulus);
  friend BigInt64 gcd(BigInt64 a, BigInt64 b);
  friend BigInt64 karatzubaMultiplication(const BigInt64 &a, const BigInt64 &b);
  bool FermasPrimalityTest(int k = 1) const;
  bool MillerRabinPrimalityTest(int k = 1) const;
  BigInt64 nextPrime(int k=1)const;
  friend BigInt64 extendedGcd(BigInt64 a, BigInt64 b, BigInt64 &x, BigInt64 &y);
  friend BigInt64 modularInverse(BigInt64 a, BigInt64 b);
  void exportTo(uint8_t *out, size_t outLen)const;
  // friend BigInt64 sqrt(BigInt64 a);
  // friend BigInt64 modularRoot(BigInt64 a, BigInt64 p);
private:
  // uint64_t is 8B, like unsigned long long
  std::vector<uint64_t> limbs;
  bool isNegative;

  // logic
  bool isSmallerThanUnsigned(const BigInt64 &b) const;
  void prefixPlusPlusUnsigned();
  void prefixMinusMinusUnsigned();

  void addUnsigned(const BigInt64 &b);

  void subtractUnsigned(const BigInt64 &b);

  friend BigInt64 longMultiplication(const BigInt64 &a, const BigInt64 &b);
  friend BigInt64 longDivision(const BigInt64 &a, const BigInt64 &b,
                               BigInt64 &remainder);
  friend BigInt64 power(BigInt64 base, BigInt64 exponent);

  BigInt10 toDecimal() const;
  void initFromString(const char *str, int n);
  void rightShift(uint64_t n);
  void leftShift(uint64_t n);
  // helpers

  void thisEqualsbBSubthis(const BigInt64 &b);
  void removeLeadingZeros();
  void insertZroes(int n);
  void generateNLimbsRandom(int limbsCnt);
  uint64_t randomLimb(uint64_t min, uint64_t max);
  void zero();
};
#endif // __BIG_INT_64__
