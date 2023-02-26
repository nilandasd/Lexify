#include "Lexify.hpp"

namespace Lexify {
  void Lexer::setRule(TokenID id, const char* regex, bool save_attr) {
    rules.push_back(Rule{id, regex, save_attr});
  }

  void Lexer::setIgnore(const char* regex) {
    ignore_rules.push_back(regex);
  }

  Token* Lexer::nextToken() {
    if (cursor < (tokens.size()))
      return &tokens[cursor++];

    lexNextLine();

    if (cursor < (tokens.size()))
      return &tokens[cursor++];

    return nullptr;
  }

  void Lexer::lexNextLine() {

  }

  // match next line

  Token* Lexer::currentToken() {
    if (tokens.size() == 0) 
      return nullptr;

    return &tokens.back();
  }
}
