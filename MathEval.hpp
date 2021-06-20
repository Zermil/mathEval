#ifndef MATH_EVAL_HPP
#define MATH_EVAL_HPP

#include <vector>
#include <algorithm>
#include <string>

namespace MathEval {
  // Declarations
  const char OPERATORS[] = {
    '+', '-', '*', '/', '(', ')'
  };

  enum class TokenType {
    NUMBER_TOKEN,
    PLUS_TOKEN,
    MINUS_TOKEN,
    MULT_TOKEN,
    DIV_TOKEN,
    OPENB_TOKEN,
    CLOSEB_TOKEN,
    BAD_TOKEN,
  };

  // Functions
  std::string ltrim(const std::string& s);
  inline bool isOperator(char c);
  inline bool isNumber(const std::string& value);
  std::string nextToken(std::string& source);
  std::vector<std::string> tokenize(std::string& expression);


  // Implementations
  std::string ltrim(const std::string& s) {
    size_t start;
    for (start = 0; s[start] == ' '; ++start);
    return s.substr(start);
  }

  inline bool isOperator(char c) {
    return std::find(std::begin(OPERATORS), std::end(OPERATORS), c) != std::end(OPERATORS);
  }

  inline bool isNumber(const std::string& value) {
    for (const char& c : value) {
      if (!(c >= '0' && c <= '9')) {
        return false;
      }
    }

    return true;
  }

  std::string nextToken(std::string& source) {
    if (source == "") {
      return "";
    }

    source = ltrim(source);
    std::string token = source;

    if (isOperator(source[0])) {
      token = source[0];
      source = source.substr(1);
      return token;
    } else {
      for (size_t i = 0; i < source.length(); ++i) {
        if (isOperator(source[i]) || source[i] == ' ') {
          token = source.substr(0, i);
          source = source.substr(i);
          return token;
        }
      }
    }

    source = "";
    return token;
  }

  std::vector<std::string> tokenize(std::string& expression) {
    std::vector<std::string> tokens;

    for (std::string token = nextToken(expression); token != ""; token = nextToken(expression)) {
      tokens.push_back(token);
    }

    return tokens;
  }
}

#endif // MATH_EVAL_HPP
