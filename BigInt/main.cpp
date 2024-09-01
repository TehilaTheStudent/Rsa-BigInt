//
//
//#include <iostream>
//#include "big_int.h"
// using namespace std;
//
// void testBigInt()
//{
//	BigInt a("123");
//	BigInt b("456");
//	BigInt c = karatsubaMult(a, b);
//
//	BigInt first("12345");
//	cout << "The number of digits"
//		<< " in first big integer = "
//		<< Length(first) << '\n';
//	BigInt second(12345);
//	if (first == second)
//	{
//		cout << "first and second are equal!\n";
//	}
//	else
//		cout << "Not equal!\n";
//	BigInt third("10000");
//	BigInt fourth("100000");
//	if (third < fourth)
//	{
//		cout << "third is smaller than fourth!\n";
//	}
//	BigInt fifth("10000000");
//	if (fifth > fourth)
//	{
//		cout << "fifth is larger than fourth!\n";
//	}
//
//	// Printing all the numbers
//	cout << "first = " << first << '\n';
//	cout << "second = " << second << '\n';
//	cout << "third = " << third << '\n';
//	cout << "fourth = " << fourth << '\n';
//	cout << "fifth = " << fifth << '\n';
//
//	// Incrementing the value of first
//	first++;
//	cout << "After incrementing the"
//		<< " value of first is : ";
//	cout << first << '\n';
//	BigInt sum;
//	sum = (fourth + fifth);
//	cout << "Sum of fourth and fifth = "
//		<< sum << '\n';
//	BigInt product;
//	product = second * third;
//	cout << "Product of second and third = "
//		<< product << '\n';
//
//	// Print the fibonacci number from 1 to 100
//	cout << "-------------------------Fibonacci"
//		<< "------------------------------\n";
//	// for (int i = 0; i <= 100; i++) {
//	//	BigInt Fib;
//	//	Fib = NthFibonacci(i);
//	//	cout << "Fibonacci " << i << " = " << Fib << '\n';
//	// }
//	// cout << "-------------------------Catalan"
//	//	<< "------------------------------\n";
//	// for (int i = 0; i <= 100; i++) {
//	//	BigInt Cat;
//	//	Cat = NthCatalan(i);
//	//	cout << "Catalan " << i << " = " << Cat << '\n';
//	// }
//
//	//// Calculating factorial of from 1 to 100
//	// cout << "-------------------------Factorial"
//	//	<< "------------------------------\n";
//	// for (int i = 0; i <= 100; i++) {
//	//	BigInt fact;
//	//	fact = Factorial(i);
//	//	cout << "Factorial of "
//	//		<< i << " = ";
//	//	cout << fact << '\n';
//	// }
//	//// This code is contributed
//	//// by Gatea David
//
//}
//
//#include <iostream>
//#include <vector>
//#include <string>
//#include <algorithm>
//
// int divideBy16(std::vector<int>& number) {
//
//	int remainder0_15 = 0;
//
//	for (int i = 0; i < number.size();i++) {
//		int digit = number[i];
//		int current = remainder0_15 * 10 + digit;
//		//how many times it conteins 16
//		number[i]=current / 16;
//		//remainder of current/16
//		remainder0_15 = current % 16;
//	}
//
//	// Remove leading zeros from the quotient
//	while (!number.empty() && number[0] == 0) {
//		number.erase(number.begin());
//	}
//
//	// The remainder is the next hexadecimal digit
//	return remainder0_15;
//}
//
// std::string convertDecimalVectorToHex(const std::vector<int>& decimalDigits)
// { 	std::vector<int> number = decimalDigits; 	std::string hexResult =
// "";
//	std::string hexDigits = "0123456789ABCDEF";
//
//	while (!number.empty()) {
//		int remainder = divideBy16(number);
//		hexResult += hexDigits[remainder];
//	}
//
//	std::reverse(hexResult.begin(), hexResult.end());
//	return hexResult.empty() ? "0" : hexResult;
//}
// std::string hexToDecimal(const std::string& hexStr) {
//	std::string decimalResult = "0";
//
//	for (char hexDigit : hexStr) {
//		// Convert the current decimal result to integer for processing
//		int carry = 0;
//		for (char& digit : decimalResult) {
//			int num = (digit - '0') * 16 + carry;
//			digit = (num % 10) + '0';
//			carry = num / 10;
//		}
//		// Handle carry if present
//		while (carry > 0) {
//			decimalResult.insert(decimalResult.begin(), (carry % 10)
//+ '0'); 			carry /= 10;
//		}
//
//		// Add the value of the current hex digit
//		int value;
//		if (hexDigit >= '0' && hexDigit <= '9') {
//			value = hexDigit - '0';
//		}
//		else if (hexDigit >= 'A' && hexDigit <= 'F') {
//			value = hexDigit - 'A' + 10;
//		}
//		else if (hexDigit >= 'a' && hexDigit <= 'f') {
//			value = hexDigit - 'a' + 10;
//		}
//
//		// Add the decimal equivalent of the hex digit to the result
//		carry = value;
//		for (int i = decimalResult.size() - 1; i >= 0; --i) {
//			int num = (decimalResult[i] - '0') + carry;
//			decimalResult[i] = (num % 10) + '0';
//			carry = num / 10;
//		}
//		// Handle carry if present after addition
//		while (carry > 0) {
//			decimalResult.insert(decimalResult.begin(), (carry % 10)
//+ '0'); 			carry /= 10;
//		}
//	}
//
//	return decimalResult;
//}
// int main()
//{
//	std::string hexNumber = "FFF"; // Example hex number
//	std::string decimalNumber = hexToDecimal(hexNumber);
//	std::cout << "Decimal: " << decimalNumber << std::endl; // Output the
// result 	std::vector<int> decimalDigits = {1,0,0,0,0 }; // Represents
// 12345 in decimal 	std::string hexResult =
// convertDecimalVectorToHex(decimalDigits); 	std::cout << "Hexadecimal: " <<
// hexResult << std::endl; // Outputs: Hexadecimal: 3039
//
//	BigInt a = "12345678";
//	BigInt b = "123";
//	a /= b;
//	std::cout << "Hello World!\n";
//}
//

#include "big_int10.h"
#include "big_int64.h"
#include "big_int_utils.h"
#include <algorithm>
#include <cstdint>
#include <iostream>
#include <random>
#include <vector>
using namespace std;

const uint64_t BASE = 0xFFFFFFFFFFFFFFFFULL; // Base 2^64 - 1

// Function to divide a large number represented by a vector of digits by BASE
uint64_t divideByBase(std::vector<uint8_t> &decNumber) {
  uint64_t remainder = 0;

  for (uint8_t &digit : decNumber) {
    // Simulate the division of a large number by BASE
    /* __uint128_t current = (__uint128_t)remainder * 10 + digit;
         digit = current / BASE;
         remainder = current % BASE;*/
  }

  // Remove leading zeros from the quotient
  while (!decNumber.empty() && decNumber[0] == 0) {
    decNumber.erase(decNumber.begin());
  }

  // Return the remainder, which is a digit in the new base
  return remainder;
}

// Convert a decimal vector to the large base (2^64 - 1)
std::vector<uint64_t>
convertDecimalVectorToBase(const std::vector<uint8_t> &decimalDigits) {
  std::vector<uint8_t> number = decimalDigits;
  std::vector<uint64_t> baseResult;

  while (!number.empty()) {
    uint64_t remainder = divideByBase(number);
    baseResult.push_back(remainder);
  }

  std::reverse(baseResult.begin(), baseResult.end());
  return baseResult;
}

int main() {

  BigInt64 a(BigInt64::CreateModes::RANDOM, 100);
  BigInt64 b(BigInt64::CreateModes::RANDOM, 100);
  START_TIMER
  BigInt64 c = a ^ b;
  END_TIMER("power")

  //   // Example: Vector representing a large number in base 10
  //   std::vector<uint8_t> decimalDigits = {
  //       1, 2, 3, 4, 5, 1, 2, 3,
  //       4, 5, 1, 2, 3, 4, 5,1,2,3,4,5,1,2,3,4,5}; // Represents 12345 in
  //       decimal

  //   // Convert to base 0xFFFFFFFFFFFFFFFF
  //   std::vector<uint64_t> baseResult =
  //   convertDecimalVectorToBase(decimalDigits);

  //   // Print the result
  //   std::cout << "Base 2^64 - 1 Result: ";
  //   for (uint64_t digit : baseResult) {
  //     std::cout << digit << " ";
  //   }
  //   std::cout << std::endl;

  //   std::cout << sizeof(long);
  //   std::cout << sizeof(long long);
  //   uint64_t a = 0xFFFFFFFFFFFFFFFF /* some value */;
  //   uint64_t b = 2 /* some value */;
  //   unsigned long long result;
  //   bool overflow_flag = false;

  //   // Using GCC/Clang built-in function to detect overflow
  //   overflow_flag = __builtin_uaddll_overflow(a, b, &result);

  //   if (overflow_flag) {
  //     std::cout << "Overflow occurred! " << result << std::endl;
  //   } else {
  //     std::cout << "No overflow. Result: " << result << std::endl;
  //   }
}