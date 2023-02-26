#pragma once

#include <vector>
#include <regex>
#include <iostream>
#include <sstream>
#include <utility>
#include <algorithm>
#include <tuple>

namespace Lexify {
  typedef uint TokenID;

  enum class Error {
    NO_ERROR = 0,
    NO_MATCH,
    END_OF_INPUT,
  };

  struct Token {
    TokenID id;
    const char* attr;
  };

  struct Rule {
    TokenID id;
    const char* regex;
    bool save_attr;
  };

  class Lexer {
    public:
      Lexer() {};
      Lexer(std::istringstream* stream) : iss(stream) {};
      ~Lexer() {};

      void setStream(std::istringstream* stream) {
        iss = stream;
        current_line = 0;
      };
      void setRule(TokenID id, const char* regex, bool save_attr_flag);
      void setIgnore(const char* regex);
      void clearRules() { rules.clear(); }
      Token* nextToken();
      Token* currentToken();
      Error getError() const { return error; };
      void logError() const;
      void setDebug(bool b) { debug = b; };

    private:
      std::regex compiled_pattern;
      std::istringstream* iss;
      std::vector<Token> tokens;
      uint cursor = 0;
      std::vector<Rule> rules;
      std::vector<const char*> ignore_rules;
      std::string currentMatch;
      Error error = Error::NO_ERROR;
      uint current_line = 0;
      bool debug = false;

      void lexNextLine();
  };
}
