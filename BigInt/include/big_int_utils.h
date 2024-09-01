#ifndef __BIG_INT_UTILS__
#define __BIG_INT_UTILS__
#include <chrono>
#include <cstdint>
#include <string>
#include <vector>
#define MAX_VAL_64 UINT64_MAX // 18446744073709551615
#define MAX_VAL_10 9
#define BITS_PER_BYTE 8

#define START_TIMER                                                            \
  auto start_timer = std::chrono::high_resolution_clock::now();
#define END_TIMER(message)                                                     \
  auto end_timer = std::chrono::high_resolution_clock::now();                  \
  std::chrono::duration<double> elapsed = end_timer - start_timer;             \
  std::cout << message << " took " << elapsed.count() << " seconds\n";

void printVector(const std::vector<uint8_t> &vec, const std::string &message);

inline uint64_t toBase64(uint64_t val) { return MAX_VAL_64 - val + 1; }

inline void adder3_64(const uint64_t &a, const uint64_t &b,
                      const uint64_t &carryIn, uint64_t &sum,
                      uint64_t &carryOut) {
  uint64_t ab = a + b;
  // note that if A+B coused overflow, the result will be smaller than both
  uint64_t out = (ab < b) ? 1 : 0;
  sum = ab + carryIn;

  carryOut = out + ((sum < ab) ? 1 : 0);
}

#if defined(_WIN32) || defined(_WIN64) // Check for Windows

#include <intrin.h> // Include intrinsics for Windows

inline void mul2Limbs64(const uint64_t a, const uint64_t b, uint64_t &high,
                        uint64_t &low) {
  low = _umul128(
      a, b, &high); // Perform 64-bit multiplication and get high and low parts
}

#elif defined(__linux__) // Check for Linux

inline void mul2Limbs64(const uint64_t a, const uint64_t b, uint64_t &high,
                        uint64_t &low) {
  __uint128_t result = static_cast<__uint128_t>(a) * b;
  low = static_cast<uint64_t>(result);
  high = static_cast<uint64_t>(result >> 64);
}

#else
#error "Unsupported platform"
#endif

inline uint64_t toBase10(uint8_t val) {
  // note that 0->0
  return (MAX_VAL_10 - val + 1) % 10;
}
inline void adder3_10(const uint8_t &a, const uint8_t &b,
                      const uint8_t &carryIn, uint8_t &sum, uint8_t &carryOut) {
  uint8_t abcarry = a + b + carryIn;
  sum = abcarry % 10;
  carryOut = abcarry / 10;
}
inline void mul2Limbs10(const uint8_t a, const uint8_t b, uint8_t &high,
                        uint8_t &low) {
  uint8_t mult = a * b;
  low = mult % 10;
  high = mult / 10;
}

inline bool isDigit10(const uint8_t a) { return a >= 0 && a <= 9; }
inline bool isCharDigit10(const uint64_t a) { return a >= '0' && a <= '9'; }
inline int bitsCount(uint64_t a) {

#if defined(_MSC_VER) // Check if compiling with Visual Studio

  int count = 0;
  while (a > 0) {
    a >>= 1;
    count++;
  }

  return count;

#else // Assume Linux or GCC/Clang

  int count = 64 - __builtin_clzll(a);
  return count;
#endif
}

#endif // __BIG_INT_UTILS__