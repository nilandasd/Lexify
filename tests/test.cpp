#include "../Lexify.hpp"
#include <sstream>
#include <iostream>

// open a file and get an fstream
// get istringstream from fstream
//
// Lexer lexer(stream);
//
// while (lexer.nextToken()) {
//   Token* token = lexer.currentToken();
//
//   if (lexer.getError()) {
//     lexer.logError();
//     break;
//   }
//
//   do something with token
//
// }

namespace Lexify {
  class Test {
    public:
      Test() {};
      ~Test() {};

      void run();

    private:
      static uint testCount;
      static uint failCount;

      void runTest(const char* testName, void (*test)());
      void basicTests();
      //void fuzzyTests();
      void results();

      static void noTextTest();
      static void noRulesTest();
      static void matchTest();
      static void noMatchTest();
      static void idTest();
      static void attrTest();
      static void precedenceTest1();
      static void precedenceTest2();
  };

  uint Test::testCount = 0;
  uint Test::failCount = 0;

  void Test::run() {
    basicTests();

    results();
  }

  void Test::basicTests() {
    std::cout << "\033[33m--- RUNNING BASIC TESTS ---\033[0m" << std::endl;

    runTest("noTextTest",     noTextTest);
    runTest("noRulesTest",    noRulesTest);
    runTest("matchTest",      matchTest);
    runTest("noMatchTest",    noMatchTest); 
    runTest("idTest",         idTest);
    runTest("attrTest",       attrTest);
    runTest("precedenceTest1", precedenceTest1);
    runTest("precedenceTest2", precedenceTest2);
  }

  void Test::results() {
    std::cout << "\033[33m--- RAN " << testCount << " TESTS ---\033[0m" << std::endl;
    std::cout << "\033[32mPASSED\033[0m " << testCount - failCount << std::endl;
    std::cout << "\033[31mFAILED\033[0m " << failCount << std::endl;
  }

  void Test::runTest(const char* testName, void (*test)()) {
    try {
      testCount++;
      test();
      std::cout << testName << ": \033[32m\tPASSED\033[0m" << std::endl;
    } catch(...) {
      failCount++;
      std::cout << testName << ": \033[31m\tFAILED\033[0m" << std::endl;
    }
  }

  void Test::noTextTest() {
    std::istringstream iss("");
    Lexify::Lexer lexer(&iss);

    lexer.setRule(0, "test", false);

    if (lexer.nextToken() != nullptr) throw std::exception();
  }

  void Test::noRulesTest() {
    std::istringstream iss("test");
    Lexify::Lexer lexer(&iss);

    if (lexer.nextToken() != nullptr) throw std::exception();
  }

  void Test::matchTest() {
    std::istringstream iss("test");
    Lexify::Lexer lexer(&iss);

    lexer.setRule(0, "test", false);

    if (lexer.nextToken() == nullptr) throw std::exception();
  }
  
  void Test::noMatchTest() {
    std::istringstream iss("test");
    Lexify::Lexer lexer(&iss);

    lexer.setRule(0, "foobar", false);
    lexer.compile();

    if (lexer.nextToken() != nullptr) throw std::exception();
  }

  void Test::idTest() {
    std::istringstream iss("foo bar");
    Lexify::Lexer lexer(&iss);

    lexer.setRule(0, "foo", false);
    lexer.setRule(1, "bar", false);

    if (lexer.nextToken() == nullptr)  throw std::exception();
    if (lexer.currentToken()->id != 0) throw std::exception();
    if (lexer.nextToken() == nullptr)  throw std::exception();
    if (lexer.currentToken()->id != 1) throw std::exception();
  }

  void Test::attrTest() {
    std::istringstream iss("banana mango");
    Lexify::Lexer lexer(&iss);

    lexer.setRule(0, "banana", true);
    lexer.setRule(1, "mango",  true);

    if (lexer.nextToken() == nullptr)           throw std::exception();
    if (lexer.currentToken()->attr != "banana") throw std::exception();
    if (lexer.nextToken() == nullptr)           throw std::exception();
    if (lexer.currentToken()->attr != "mango")  throw std::exception();
  }

  void Test::precedenceTest1() {
    std::istringstream iss("foobar");
    Lexify::Lexer lexer(&iss);

    lexer.setRule(0, "foo",    true);
    lexer.setRule(1, "foobar", true);

    if (lexer.nextToken() == nullptr)           throw std::exception();
    if (lexer.currentToken()->id != 1)          throw std::exception();
    if (lexer.currentToken()->attr != "foobar") throw std::exception();
  }

  void Test::precedenceTest2() {
    std::istringstream iss("foobar");
    Lexify::Lexer lexer(&iss);

    lexer.setRule(0, "foo",    true);
    lexer.setRule(1, "^[a-z]+", true);

    if (lexer.nextToken() == nullptr)           throw std::exception();
    if (lexer.currentToken()->id != 1)          throw std::exception();
    if (lexer.currentToken()->attr != "foobar") throw std::exception();
  }
}

int main() {
  Lexify::Test test;
  test.run();
  return 0;
}
