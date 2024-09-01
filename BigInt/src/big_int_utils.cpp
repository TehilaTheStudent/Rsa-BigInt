#include "big_int_utils.h"
#include <iostream>

void printVector(const std::vector<uint8_t> &vec, const std::string &message) {
  std::cout << message << ": ";
  std::cout << "[";
  for (size_t i = 0; i < vec.size(); ++i) {
    std::cout << static_cast<int>(vec[i]); // Cast to int for correct printing
    if (i < vec.size() - 1) {
      std::cout << ", ";
    }
  }
  std::cout << "]" << std::endl;
}
