#include "big_int64.h"
#include "big_int_utils.h"
#include <algorithm>
#include <random>

using namespace std;
BigInt64::BigInt64(const string &str) {
  initFromString(str.c_str(), str.size());
}

BigInt64::BigInt64(const char *str) { initFromString(str, std::strlen(str)); }

BigInt64::BigInt64(const uint8_t *str, size_t strLen) : isNegative(false) {
  if (strLen == 0) {
    limbs.push_back(0);
    return;
  }
  int fullLimbs = strLen / 8;
  int msbParts = strLen % 8;
  int limbsCnt = fullLimbs + (msbParts + 7) / 8;
  limbs.reserve(limbsCnt);
  for (int i = 0; i < fullLimbs; i++) {
    int index = strLen - 1 - i * 8;
    // little endian-> {0,1,2,3,4,5,6,7} becones 0x 07 06 05 04 03 02 01 00
    limbs.push_back((static_cast<uint64_t>(str[index]) << 56) |
                    (static_cast<uint64_t>(str[index - 1]) << 48) |
                    (static_cast<uint64_t>(str[index - 2]) << 40) |
                    (static_cast<uint64_t>(str[index - 3]) << 32) |
                    (static_cast<uint64_t>(str[index - 4]) << 24) |
                    (static_cast<uint64_t>(str[index - 5]) << 16) |
                    (static_cast<uint64_t>(str[index - 6]) << 8) |
                    static_cast<uint64_t>(str[index - 7]));
  }
  if (msbParts != 0) {
    uint64_t msb = 0;
    for (int i = 0; i < msbParts; i++) {
      msb |= static_cast<uint64_t>(str[i]) << (i * 8);
    }
    limbs.push_back(msb);
  }
  removeLeadingZeros();
}

BigInt64::BigInt64(std::vector<uint64_t> v, bool isNegative)
    : limbs(v), isNegative(isNegative) {}

BigInt64::BigInt64(CreateModes mode, int bits) : isNegative(false) {
  if (bits >= 64)
    generateNLimbsRandom(bits / 64);
  bits = bits % 64; // 0-63
  if (bits != 0) {
    uint64_t one = static_cast<uint64_t>(1);
    limbs.push_back(randomLimb(0, (one << bits) - 1) | (one << (bits - 1)));
  } else
    limbs.back() |= 0x8000000000000000ULL;
}

BigInt64::BigInt64(CreateModes mode, uint64_t min, const BigInt64 &max)
    : isNegative(false) {

  if (max.limbsCount() > 1)
    generateNLimbsRandom(max.limbsCount() - 1);
  else if (max.isSmallerThanUnsigned(min))
    throw invalid_argument("min must be <= max");
  uint64_t number = randomLimb(min, max.limbs.back());
  if (number != 0 || max.limbsCount() == 1)
    limbs.push_back(number);
}

BigInt64::BigInt64(long long val) {
  isNegative = false;
  if (val == 0) {
    limbs.push_back(0);
    return;
  }
  if (val < 0) {
    isNegative = true;
    val *= -1;
  }
  limbs.push_back(val);
}
BigInt64::BigInt64(bool isNegative) : BigInt64(0) {
  this->isNegative = isNegative;
}

BigInt64::BigInt64(uint64_t uval) {
  isNegative = false;
  limbs.push_back(uval);
}

BigInt64::BigInt64(int val) : BigInt64(static_cast<long long>(val)) {}

bool BigInt64::operator==(const BigInt64 &b) const {
  // O(n)
  //  ans= this==b
  return limbs == b.limbs && isNegative == b.isNegative;
}

bool BigInt64::operator!=(const BigInt64 &b) const { return !(*this == b); }

bool BigInt64::operator<(const BigInt64 &b) const {
  // ans= this<b
  if (isNegative && !b.isNegative)
    return true;
  if (!isNegative && b.isNegative)
    return false;
  // same sign
  if (isNegative)
    return b.isSmallerThanUnsigned(*this);
  else
    return isSmallerThanUnsigned(b);
}

bool BigInt64::operator>(const BigInt64 &b) const {
  // ans=this>b
  return b < *this;
}

bool BigInt64::operator<=(const BigInt64 &b) const {
  // ans=this<=b
  return !(b < *this);
}

bool BigInt64::operator>=(const BigInt64 &b) const {
  // ans=this>=b
  return !(*this < b);
}

BigInt64 &BigInt64::operator++() {
  // ++this;
  if (!isNegative)
    prefixPlusPlusUnsigned();
  else {
    prefixMinusMinusUnsigned();
    if (isZero())
      //-1 -> 0
      isNegative = false;
  }
  return *this;
}

BigInt64 BigInt64::operator++(int) {
  // ans=this++;
  BigInt64 temp = *this;
  ++*this;
  return temp;
}

BigInt64 &BigInt64::operator--() {
  // --this;
  if (isNegative)
    prefixPlusPlusUnsigned();
  else {
    if (isZero()) {
      // 0 -> -1
      limbs[0] = 1;
      isNegative = true;
    } else
      prefixMinusMinusUnsigned();
  }
  return *this;
}

BigInt64 BigInt64::operator--(int) {
  // ans=this--;
  BigInt64 temp = *this;
  --*this;
  return temp;
}

BigInt64 &BigInt64::operator+=(const BigInt64 &b) {
  // this+=b;
  if (isNegative == b.isNegative) // same sign
    addUnsigned(b);
  else {
    if (isNegative && !b.isSmallerThanUnsigned(*this) ||
        !isNegative && isSmallerThanUnsigned(b)) {
      // -3 + +4/-3 + +3 ||  +3 + -4
      thisEqualsbBSubthis(b);
      isNegative = b.isNegative;
    } else                 // -4 + +3 ||  +3 + -3 / +4 + -3
      subtractUnsigned(b); // this-=b
  }
  return *this;
}

BigInt64 BigInt64::operator+(const BigInt64 &b) const {
  // c=this+b
  BigInt64 c(*this);
  c += b;
  return c;
}

BigInt64 &BigInt64::operator-=(const BigInt64 &b) {
  // this-=b;
  if (isNegative != b.isNegative) // -this - +b / +a - -b
    addUnsigned(b);
  else {
    if (isNegative && !b.isSmallerThanUnsigned(*this) ||
        !isNegative && (isSmallerThanUnsigned(b))) {
      // -3 - -4/ -4 - -4 ||  +3 - +7
      thisEqualsbBSubthis(b);
      isNegative = !b.isNegative;
    } else                 //  -4 - -3 ||+4 - +3/ +3 - +3
      subtractUnsigned(b); // this-=b
  }
  return *this;
}

BigInt64 BigInt64::operator-(const BigInt64 &b) const {
  // c=this+b
  BigInt64 c(*this);
  c -= b;
  return c;
}

BigInt64 &BigInt64::operator*=(const BigInt64 &b) {
  // this*=b;
  *this = longMultiplication(*this, b);
  //*this = karatzubaMultiplication(*this, b);
  return *this;
}

BigInt64 BigInt64::operator*(const BigInt64 &b) const {
  // c=this*b
  BigInt64 c(*this);
  c *= b;
  return c;
}
BigInt64 &BigInt64::operator/=(const BigInt64 &b) {
  // this/=b;
  BigInt64 remainder;
  *this = longDivision(*this, b, remainder);
  return *this;
}

BigInt64 BigInt64::operator/(const BigInt64 &b) const {
  /// c=a/b;
  BigInt64 c(*this);
  c /= b;
  return c;
}

BigInt64 &BigInt64::operator%=(const BigInt64 &b) {
  // this%=b;
  BigInt64 remainder;
  longDivision(*this, b, remainder);
  *this = remainder;
  return *this;
}

BigInt64 BigInt64::operator%(const BigInt64 &b) const {
  /// c=a%b;
  BigInt64 c(*this);
  c %= b;
  return c;
}
BigInt64 &BigInt64::operator^=(const BigInt64 &b) {
  // this^=b;
  *this = power(*this, b);
  return *this;
}

BigInt64 BigInt64::operator^(const BigInt64 &b) const {
  // c=this^b
  BigInt64 c(*this);
  c ^= b;
  return c;
}

BigInt64 &BigInt64::operator>>=(uint64_t n) {
  // this>>=b;
  rightShift(n);
  return *this;
}

BigInt64 BigInt64::operator>>(uint64_t n) const {
  // c=this>>b;
  BigInt64 c(*this);
  c >>= n;
  return c;
}
BigInt64 &BigInt64::operator<<=(uint64_t n) {
  // this<<=b;
  leftShift(n);
  return *this;
}

BigInt64 BigInt64::operator<<(uint64_t n) const {
  // c=this<<b;
  BigInt64 c(*this);
  c <<= n;
  return c;
}

std::ostream &operator<<(std::ostream &out, const BigInt64 &a) {
  if (a.limbsCount() == 1) {
    if (a.isNegative)
      out << "-";
    out << a.limbs[0];
  }

  else {
    BigInt10 decimal = a.toDecimal();
    out << decimal;
  }
  return out;
}

bool BigInt64::isSmallerThanUnsigned(const BigInt64 &b) const {
  // O(n)
  //  ans=a<b
  if (limbsCount() > b.limbsCount())
    // a is longer
    return false;
  if (limbsCount() < b.limbsCount())
    // b is longer
    return true;
  // same length
  for (int i = limbsCount() - 1; i >= 0; i--)
    if (limbs[i] != b.limbs[i])
      return limbs[i] < b.limbs[i];
  // they are equal
  return false;
}

void BigInt64::prefixPlusPlusUnsigned() {
  //++a
  int i, n = limbsCount();
  // zero alll the MAX_VAL_64 lsb's
  for (i = 0; i < n && limbs[i] == MAX_VAL_64; i++) {
    limbs[i] = 0;
  }
  if (i == n)
    // it was all MAX_VAL_64
    limbs.push_back(1);
  else
    limbs[i]++;
}

void BigInt64::prefixMinusMinusUnsigned() {
  //--a
  if (isZero())
    throw std::invalid_argument("cant --0");
  int i = 0, n = limbsCount();
  // starting zeros case 0 0 0 X
  while (i < n && limbs[i] == 0) {
    limbs[i] = MAX_VAL_64;
    i++;
  }
  limbs[i]--; // subtruct the first valid limb
  // remove leading zero if exists
  if (limbs[i] == 0 && i != 0)
    limbs.pop_back();
}

void BigInt64::addUnsigned(const BigInt64 &b) {
  // O(n)
  // this+=b
  int n = limbsCount(), m = b.limbsCount();
  uint64_t carry = 0; //(1/0)
  uint64_t bLimb;
  if (n < m) {
    limbs.resize(m);
    n = m;
  }
  for (int i = 0; i < n; i++) {
    bLimb = i < m ? b.limbs[i] : 0;
    adder3_64(limbs[i], bLimb, carry, limbs[i], carry);
  }
  if (carry == 1)
    limbs.push_back(carry);
}

void BigInt64::subtractUnsigned(const BigInt64 &b) {
  // O(n)
  // this-=b
  if (this->isSmallerThanUnsigned(b))
    // make sure this>=b;
    throw std::invalid_argument("cant -: this < b");
  int n = limbsCount(), m = b.limbsCount();
  int borrow = 0; //(1/0)
  uint64_t bLimb;

  for (int i = 0; i < n; i++) {
    bLimb = i < m ? b.limbs[i] : 0;
    if ((borrow == 1 && limbs[i] == 0) || (limbs[i] - borrow < bLimb)) {
      // need to borrow from next limb
      limbs[i] = limbs[i] + toBase64(bLimb) - borrow;
      borrow = 1;
    } else {
      limbs[i] = limbs[i] - bLimb - borrow;
      borrow = 0;
    }
  }
  // remove leading zeros if exists
  removeLeadingZeros();
}
BigInt64 longMultiplication(const BigInt64 &a, const BigInt64 &b) {
  // O(n^2)
  // this*=b
  if (a.isZero() || b.isZero()) {
    return BigInt64();
  }
  int n = a.limbsCount(), m = b.limbsCount();
  std::vector<uint64_t> result(n + m, 0);
  uint64_t high, low, carry;

  for (int i = 0; i < n; i++) {
    carry = 0;
    for (int j = 0; j < m; j++) {
      mul2Limbs64(a.limbs[i], b.limbs[j], high, low);
      adder3_64(low, result[i + j], carry, result[i + j], carry);
      carry += high;
    }
    // Handle any remaining carry after the inner loop
    result[i + m] = carry;
  }

  BigInt64 res;
  res.limbs = std::move(result);
  res.removeLeadingZeros(); // Remove leading zeros
  res.isNegative = a.isNegative != b.isNegative;
  return res;
}
bool is_divisible_by_small_primes(const BigInt64 &n) {
  std::vector<int> small_primes = {
      2,   3,   5,   7,   11,  13,  17,  19,  23,  29,  31,  37,  41,  43,
      47,  53,  59,  61,  67,  71,  73,  79,  83,  89,  97,  101, 103, 107,
      109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181,
      191, 193, 197, 199, 211, 223, 227, 229, 233, 239, 241, 251, 257, 263,
      269, 271, 277, 281, 283, 293, 307, 311, 313, 317, 331, 337, 347, 349,
      353, 359, 367, 373, 379, 383, 389, 397, 401, 409, 419, 421, 431, 433,
      439, 443, 449, 457, 461, 463, 467, 479, 487, 491, 499};

  for (int prime : small_primes)
    if (n % prime == 0)
      return true;
  return false;
}
bool BigInt64::FermasPrimalityTest(int k) const {
  if (*this == 2)
    return true;

  if (*this <= 1 || isEven())
    return false;
  if (is_divisible_by_small_primes(*this))
    return false;
  while (k-- > 0) {
    // assume n is a prime number
    // pick randon integer a, such that 1<a<n-1
    BigInt64 a(CreateModes::RANDOM, 2, *this - 2);
    // check if n is divisible by a
    if (gcd(*this, a) != 1)
      return false;

    // Fermat's little theorem- a^(prime-1)%prime !=1
    if (modularExponentiation(a, *this - 1, *this) != 1)
      return false;
  }
  // cout << *this << " is a prime number." << endl;
  return true;
}

bool BigInt64::MillerRabinPrimalityTest(int k) const {
  if (*this == 2)
    return true;
  if (*this <= 1 || isEven())
    return false;
  if (is_divisible_by_small_primes(*this))
    return false;
  // Write n-1 as 2^s * d where d is odd
  BigInt64 s = 0;
  BigInt64 d = *this - 1;
  while (d.isEven()) {
    d >>= 1;
    s++;
  }
  while (k-- > 0) {
    // pick randon integer a, such that 1<a<n-1
    BigInt64 a(CreateModes::RANDOM, 2, *this - 2);
    BigInt64 x = modularExponentiation(a, d, *this);
    if (x == 1 || x == *this - 1)
      continue; // pass round
    bool isComposite = true;
    for (BigInt64 i = 0; i < s - 1; i++) {
      x = modularExponentiation(x, 2, *this);
      if (x == *this - 1) { // x^2 mod n=n-1
        isComposite = false;
        break; // pass round
      }
    }
    if (isComposite)
      return false; // failed test
  }
  return true;
}

BigInt64 BigInt64::nextPrime(int k) const {
  if (*this < 2)
    return 2;
  BigInt64 res = *this;
  if (res.isEven())
    res++;

  while (!res.FermasPrimalityTest(k)) {
    res += 2;
  }

  return res;
}

void BigInt64::exportTo(uint8_t *out, size_t outLen) const {
  // int msbBytes = (::bitsCount(limbs.back()) + 7) / 8;
  // int innerLimbsBytes = (limbsCount() - 1) * 8;
  if (outLen < limbsCount() * 8)
    //  if ((msbBytes + innerLimbsBytes) > outLen)
    throw std::runtime_error("Not enough space in output buffer");

  for (int i = 0; i < limbsCount(); i++)
    for (int j = 0; j < 8; j++)
      out[outLen - 1 - i * 8 - (7 - j)] =
          static_cast<uint8_t>(limbs[i] >> (j * 8));
  // uint64_t msb = limbs.back();
  // for (int i = 0; i < msbBytes; i++)
  //   out[outLen - innerLimbsBytes - (msbBytes - i)] =
  //       static_cast<uint8_t>(msb >> (i * 8));
}

int BigInt64::bitsCount() const {

  if (isZero())
    return 0;

  // Count bits in the most significant limb manually
  uint64_t mostSignificantLimb = limbs.back();
  int inMsb = ::bitsCount(mostSignificantLimb);

  return inMsb + (limbsCount() - 1) * 64;
}
void BigInt64::thisEqualsbBSubthis(const BigInt64 &b) {
  BigInt64 copyB(b);
  copyB.subtractUnsigned(*this); // copyB-=this
  *this = std::move(copyB);
}
void BigInt64::removeLeadingZeros() {
  while (limbs.size() > 1 && limbs.back() == 0) {
    limbs.pop_back();
  }
}
void BigInt64::insertZroes(int n) { limbs.insert(limbs.begin(), n, 0); }
void BigInt64::generateNLimbsRandom(int limbsCnt) {
  if (limbsCnt < 1)
    throw std::invalid_argument("limbsCnt less than 1");
  // Create a random number generator
  std::random_device rd; // Seed for the random number engine
  std::mt19937_64 gen(
      rd()); // Mersenne Twister random number engine for 64-bit integers

  // Define the range of the random numbers
  std::uniform_int_distribution<uint64_t> distrib(0, UINT64_MAX);

  limbs.reserve(limbsCnt);

  for (int i = 0; i < limbsCnt; i++) {
    uint64_t number = distrib(gen);
    limbs.push_back(number);
  }
}
uint64_t BigInt64::randomLimb(uint64_t min, uint64_t max) {
  if (min > max)
    throw std::invalid_argument("cant random limb");
  // Generate a random number between 0 and max
  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<uint64_t> distrib(min, max);
  return distrib(gen);
}
void BigInt64::zero() {
  limbs.clear();
  limbs.push_back(0);
  isNegative = false;
}
BigInt64 longDivision(const BigInt64 &a, const BigInt64 &b,
                      BigInt64 &remainder) {
  // c=a/b;
  if (b.isZero())
    throw std::invalid_argument("Invalid input: non-digit character");
  remainder.isNegative = false;
  if (a.isSmallerThanUnsigned(b)) {
    remainder.limbs = a.limbs;
    return BigInt64();
  }
  remainder.limbs.clear();
  std::vector<uint64_t> quotient;

  for (int i = a.limbsCount() - 1; i >= 0; i--) { // msb-lsb

    remainder.limbs.insert(remainder.limbs.begin(),
                           a.limbs[i]); // insert msb as lsb
    if (remainder.limbsCount() > 1 && remainder.limbs.back() == 0)
      remainder.limbs.pop_back(); // remove leading zero
    uint64_t times = 0;
    if (!remainder.isSmallerThanUnsigned(b)) {
      uint64_t left = 1, right = MAX_VAL_64;

      // after inserting next limb, most times t contains b is max value
      // of singe limb, find in range of [0...max]
      while (left <= right) {
        uint64_t mid = left + (right - left) / 2;
        if (!remainder.isSmallerThanUnsigned(b * mid)) { // b*mid<=remainder
          times = mid;
          if (left == right)
            break;
          left = mid + 1;
        } else {
          right = mid - 1;
        }
      }
      remainder.subtractUnsigned(b * times);
    }

    if (quotient.size() > 0 || times > 0)
      quotient.push_back(times);
  }

  std::reverse(quotient.begin(), quotient.end());
  BigInt64 quoti(a.isNegative != b.isNegative);
  quoti.limbs = std::move(quotient);
  return quoti;
}
BigInt64 power(BigInt64 base, BigInt64 exponent) {
  if (exponent.isNegative)
    throw std::invalid_argument("Invalid input: negative exponent");
  BigInt64 result(1);

  while (!exponent.isZero()) {
    if (!exponent.isEven()) {
      result *= base;
    }
    base *= base;
    exponent >>= 1;
  }
  return result;
}
BigInt10 BigInt64::toDecimal() const {
  BigInt10 decimal;
  BigInt10 base(MAX_VAL_64);
  base++;
  for (int i = limbsCount() - 1; i >= 0; i--) {
    BigInt10 limb = limbs[i];
    decimal *= base;
    decimal += limb;
  }
  decimal.isNegative = isNegative;
  return decimal;
}

void BigInt64::initFromString(const char *str, int n) {
  BigInt10 decimal = str;
  isNegative = decimal.isNegative;
  BigInt10 base = MAX_VAL_64;
  base++;
  if (decimal.isZero())
    limbs.push_back(0);
  else
    while (!decimal.isZero()) {
      limbs.push_back((decimal % base).toU64());
      decimal /= base;
    }
}

BigInt64 modularExponentiation(BigInt64 base, BigInt64 exponent,
                               const BigInt64 &modulus) {
  BigInt64 res(1);
  base %= modulus;
  while (!exponent.isZero()) {
    if (!exponent.isEven())
      res = (res * base) % modulus;
    base = (base * base) % modulus;
    exponent >>= 1;
  }
  return res;
}

BigInt64 gcd(BigInt64 a, BigInt64 b) {

  while (!b.isZero()) {
    BigInt64 copyB = b;
    b = a % b;
    a = copyB;
  }
  return a;
}

BigInt64 karatzubaMultiplication(const BigInt64 &a, const BigInt64 &b) {
  int n = a.limbsCount(), m = b.limbsCount();
  if (n == 1 || m == 1)
    return longMultiplication(a, b);
  const BigInt64 *longerX, *shorterY;
  int xLen, yLen;
  if (n >= m) {
    longerX = &a;
    xLen = n;
    shorterY = &b;
    yLen = m;
  } else {
    longerX = &b;
    xLen = m;
    shorterY = &a;
    yLen = n;
  }
  int xLsbHalf = xLen / 2;
  int yLsbHalf = min(xLsbHalf, yLen);
  BigInt64 xM, xL, yL, yM;
  xM.limbs.reserve(xLen - xLsbHalf);
  xL.limbs.reserve(xLsbHalf);
  yM.limbs.reserve(yLen - yLsbHalf);
  yL.limbs.reserve(yLsbHalf);
  // Msb Lsb
  //  xM aL =longer <-[L M], [xL ......  xM]
  //  yM bL =shorter<-[L M], [yL  yM]
  xL.limbs.assign(longerX->limbs.begin(), longerX->limbs.begin() + xLsbHalf);
  xL.removeLeadingZeros();
  xM.limbs.assign(longerX->limbs.begin() + xLsbHalf,
                  longerX->limbs.begin() + xLen);
  yL.limbs.assign(shorterY->limbs.begin(), shorterY->limbs.begin() + yLsbHalf);
  yL.removeLeadingZeros();
  if (yLen - yLsbHalf != 0)
    yM.limbs.assign(shorterY->limbs.begin() + yLsbHalf,
                    shorterY->limbs.begin() + yLen);

  BigInt64 sum4Mult = karatzubaMultiplication(xL + xM, yL + yM);
  BigInt64 xyL = karatzubaMultiplication(xL, yL);
  BigInt64 xyM = karatzubaMultiplication(xM, yM);
  BigInt64 xLyM_xMyL = sum4Mult - xyL - xyM;
  xyM.insertZroes(xLsbHalf * 2);
  xLyM_xMyL.insertZroes(xLsbHalf);
  BigInt64 res = xyM + xLyM_xMyL + xyL;
  res.isNegative = a.isNegative != b.isNegative;
  return res;
}
void BigInt64::rightShift(uint64_t n) { //--->
  if (n >= bitsCount()) {
    zero();
    return;
  }
  uint64_t shiftEachLimb = n % 64;
  uint64_t dropLimbs = n / 64;
  if (shiftEachLimb != 0) {
    for (int i = 0; i < limbsCount() - 1 - dropLimbs; i++) // lsb...msb
      limbs[i] = limbs[i + dropLimbs] >> shiftEachLimb |
                 limbs[i + dropLimbs + 1] << (64 - shiftEachLimb);
    limbs[limbsCount() - 1 - dropLimbs] >>= shiftEachLimb;
    limbs.resize(limbsCount() - dropLimbs);
    if (limbs.back() == 0 && !isZero())
      limbs.pop_back();
  } else
    limbs.erase(limbs.begin(), limbs.begin() + dropLimbs);
}

void BigInt64::leftShift(uint64_t n) { //<---
  if (n >= bitsCount()) {
    zero();
    return;
  }
  uint64_t shiftEachLimb = n % 64;
  uint64_t dropLimbs = n / 64;
  if (shiftEachLimb != 0) {
    for (int i = limbsCount() - 1; i > dropLimbs; i--) { // msb-lst
      limbs[i] = limbs[i - dropLimbs] << shiftEachLimb |
                 limbs[i - dropLimbs - 1] >> (64 - shiftEachLimb);
    }
    limbs[0] <<= shiftEachLimb;
  } else {
    // sycl here?
    for (int i = limbsCount() - 1; i >= dropLimbs; i--)
      limbs[i] = limbs[i - dropLimbs];
    for (int i = dropLimbs - 1; i >= 0; i--)
      limbs[i] = 0;
  }

  removeLeadingZeros();
}
BigInt64 extendedGcd(BigInt64 a, BigInt64 b, BigInt64 &x, BigInt64 &y) {
  x = 1;
  y = 0;
  BigInt64 x1 = 0, y1 = 1;
  BigInt64 q, temp;

  while (b != 0) {
    q = a / b;
    temp = b;
    b = a % b;
    a = temp;

    temp = x1;
    x1 = x - q * x1;
    x = temp;

    temp = y1;
    y1 = y - q * y1;
    y = temp;
  }

  return a;
}
BigInt64 modularInverse(BigInt64 a, BigInt64 b) {
  BigInt64 x, y;
  BigInt64 gcdResult = extendedGcd(a, b, x, y);
  if (gcdResult != 1)
    throw std::invalid_argument("Modular inverse does not exist.");
  return (x % b + b) % b; // Ensure result is positive
}

// BigInt64 sqrt(BigInt64 a) {
//   if (a < 0)
//     throw std::invalid_argument("Cannot compute square root of a negative
//     number.");
//   BigInt64 x = a;
//   BigInt64 y = (a + 1) / 2;
//   while (x < y) {
//     x = y;
//     y = (a / y + y) / 2;
//   }
//   return x;
// }

// BigInt64 modularRoot(BigInt64 a, BigInt64 p) {
//   if (p == 2)
//     return a % 2 == 0? 0 : 1;
//   if (p % 2 == 0) {
//     BigInt64 x = modularRoot(a, p / 2);
//     return modularExponentiation(x, x, p);
//   } else {
//     BigInt64 x = modularRoot(a, (p - 1) / 2);
//     return modularExponentiation(x, x, p) * a % p;
//   }
//  }
