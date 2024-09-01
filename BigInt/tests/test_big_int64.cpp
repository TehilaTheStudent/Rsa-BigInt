#include "big_int64.h"
#include "big_int_utils.h"
#include <gtest/gtest.h>
#include <iostream>
using namespace std;

TEST(BigInt64, importAndExport) {
  std::vector<uint8_t> input = {0,1,2,3,4,5,6,7,8};
  BigInt64 imported(input.data(), input.size());

  // Export to verify
  std::vector<uint8_t> output(input.size()+8);
  imported.exportTo(output.data()+7, output.size());

  EXPECT_EQ(input , output);
}
TEST(BigInt64, Multiplication) {
  BigInt64 a("-654365436534654365436543654365436436543654363");
  BigInt64 b("6543654365436536543654365436536436543654365436543653");
  BigInt64 expected("-428194124537077593810686205532132186187392109694483768577"
                    "3005078438059467530494444314064593408039");
  BigInt64 c = a * b;
  EXPECT_EQ(expected, c);
}
TEST(BigInt64, Division) {
  BigInt64 a(
      "-67896789678968666666666666666666666666666666666666666666666679867896");
  BigInt64 b("6543654365436536543654365436536436543654365436543653");

  BigInt64 expectedQuotient("-10375974323704851");
  BigInt64 expectedRemainder(
      "3480525494090974591903481958475999714137731530507193");

  BigInt64 quotient = a / b;
  BigInt64 remainder = a % b;

  EXPECT_EQ(expectedQuotient, quotient);
  EXPECT_EQ(expectedRemainder, remainder);
}
TEST(BigInt64, GCD) {
  BigInt64 a("58349054854737865786576953765376537896378965478653278964378956378"
             "95326879");
  BigInt64 b("-5834905485473786578657695376537653789637896547865327896437895637"
             "895326879000");
  BigInt64 expected("5834905485473786578657695376537653789637896547865327896437"
                    "895637895326879");

  EXPECT_EQ(expected, gcd(a, b));
}
TEST(BigInt64, randomBits) {
  BigInt64 a(BigInt64::CreateModes::RANDOM, 64);
  BigInt64 b(BigInt64::CreateModes::RANDOM, 1, 1);
  EXPECT_EQ(a.bitsCount(), 64);
  EXPECT_EQ(b, 1);
}
TEST(BigInt64, modularExponentiation) {
  BigInt64 a("5435432543254325432");
  BigInt64 b("543254");

  BigInt64 res = modularExponentiation(a, b - 1, b);
  BigInt64 actual = "88198";
  EXPECT_EQ(res, actual);
}
TEST(BigInt64, karatzubaMultiplication) {
  BigInt64 a(BigInt64::CreateModes::RANDOM, 4096);
  BigInt64 b(BigInt64::CreateModes::RANDOM, 4096);

  BigInt64 longMult = longMultiplication(a, b);
  START_TIMER
  BigInt64 karaMult = karatzubaMultiplication(a, b);
  END_TIMER("karatzuba multiplication")
  EXPECT_EQ(karaMult, longMult);
}
TEST(BigInt64, longMultiplication) {
  BigInt64 a(BigInt64::CreateModes::RANDOM, 4096);
  BigInt64 b(BigInt64::CreateModes::RANDOM, 4096);
  START_TIMER
  BigInt64 longMult = longMultiplication(a, b);
  END_TIMER("long multiplication")

  BigInt64 karaMult = karatzubaMultiplication(a, b);

  EXPECT_EQ(karaMult, longMult);
}
TEST(BigInt64, nextPrime2048) {
  // watch out from this long test!
  // it takes 3 minutes!
  //😰😰😰😰😰😰😰😰😰
  // BigInt64 a = BigInt64(BigInt64::CreateModes::RANDOM,2048);
  BigInt64 a =
      "255103232852919265490572597742411399997230821960224162728733259372788100"
      "848712954079287761237686640492149121213081070249076935595989023274625564"
      "482318842883685294622736546257421631123357356972164235516457505306469159"
      "751740784743263340887348221750657258734613305475557616350358104709313277"
      "781643294070544561629820382030424035913807729751447629927800767040722949"
      "551094956818837365118494456430926441472716508967465896636887731751274279"
      "342790922974124983570382338833371354179405865425484493862322238067284075"
      "433566839206066502335985470474573358464932990238417480316838640746201490"
      "26165096136526942001642396023299578308768";

  cout << " 2048 bits:" << endl << a << endl;
  START_TIMER
  BigInt64 n = a.nextPrime();
  END_TIMER("next prime 2048")
  BigInt64 realNext = "25510323285291926549057259774241139999723082196022416272"
                      "87332593727881008487"
                      "12954079287761237686640492149121213081070249076935595989"
                      "02327462556448231884"
                      "28836852946227365462574216311233573569721642355164575053"
                      "06469159751740784743"
                      "26334088734822175065725873461330547555761635035810470931"
                      "32777816432940705445"
                      "61629820382030424035913807729751447629927800767040722949"
                      "55109495681883736511"
                      "84944564309264414727165089674658966368877317512742793427"
                      "90922974124983570382"
                      "33883337135417940586542548449386232223806728407543356683"
                      "92060665023359854704"
                      "74573358464932990238417480316838640746201490261650961365"
                      "26942001642396023299"
                      "578311313";
  cout << n << endl;
  cout << "gap:" << n - a << endl;
  EXPECT_EQ(n, realNext);
  EXPECT_TRUE(n.FermasPrimalityTest());
  EXPECT_TRUE(n.MillerRabinPrimalityTest());
}
TEST(BigInt64, nextPrime1024) {
  // watch out from this long test!
  // it takes 3 minutes!
  //😰😰😰😰😰😰😰😰😰
  BigInt64 a = "149850771096313008630641747195545090291042579671124986807081015"
               "821717840698748928973964882268170336697631753534109646521940731"
               "722420534581970385986561352391928703629650838089531469337149976"
               "703193673778860430276778398711828934956265846857601931063362118"
               "405758317713825532612790336772850247302677014935636940174";
  START_TIMER
  BigInt64 n = a.nextPrime();
  END_TIMER("next prime 1024")
  BigInt64 realNext =
      "149850771096313008630641747195545090291042579671124986807081015821717840"
      "698748928973964882268170336697631753534109646521940731722420534581970385"
      "986561352391928703629650838089531469337149976703193673778860430276778398"
      "711828934956265846857601931063362118405758317713825532612790336772850247"
      "302677014935636941149";
  cout << n << endl;
  cout << "gap:" << n - a << endl;
  EXPECT_EQ(n, realNext);
  EXPECT_TRUE(n.FermasPrimalityTest());
  EXPECT_TRUE(n.MillerRabinPrimalityTest());
}
TEST(BigInt64, ShiftByBitsCount) {
  BigInt64 number(
      "12345678543543254325432254325432254325432543290123456789000");
  int bitsCount = number.bitsCount();
  BigInt64 resRight = number >> 4;
  BigInt64 expectedRight = number / 16;
  BigInt64 resLeft = number << 4;
  BigInt64 expectedLeft = number * 16;

  EXPECT_EQ(bitsCount - 4, resRight.bitsCount());
  EXPECT_EQ(bitsCount + 4, resLeft.bitsCount());

  EXPECT_EQ(resRight, expectedRight);
  EXPECT_EQ(resLeft, expectedLeft);
}

