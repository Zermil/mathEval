#ifndef MATH_EVAL_HPP
#define MATH_EVAL_HPP

#include <vector>
#include <algorithm>
#include <string>

namespace MathEval {
  // Declarations
  struct Operator {
    char identifier;
    int precedence;
  };

  class Tokenizer {
  public:
    std::string getNextToken(std::string& source);

  private:
    bool allowNegative_ = true;
  };

  const Operator OPERATORS[] = {
    { '+', 0 },
    { '-', 0 },
    { '*', 1 },
    { '/', 1 }
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

  // Functions (wrappers and utilities)
  std::string ltrim(const std::string& s);
  inline bool isOperator(char c);
  inline bool isNumber(const std::string& value);
  inline bool isNumber(const char& c);
  std::vector<std::string> tokenize(std::string& expression);


  // Implementations
  std::string ltrim(const std::string& s) {
    size_t start;
    for (start = 0; s[start] == ' '; ++start);
    return s.substr(start);
  }

  inline bool isOperator(char c) {
    for (const Operator& op : OPERATORS) {
      if (op.identifier == c) {
        return true;
      }
    }

    return false;
  }

  inline bool isNumber(const char& c) {
    if (!(c >= '0' && c <= '9')) {
      return false;
    }

    return true;
  }

  inline bool isNumber(const std::string& value) {
    for (const char& c : value) {
      if (!(c >= '0' && c <= '9')) {
        return false;
      }
    }

    return true;
  }

  std::string Tokenizer::getNextToken(std::string& source) {
    if (source == "") {
      return "";
    }

    source = ltrim(source);
    std::string token = source;

    // Hangle negative numbers
    if (source[0] == '-' && allowNegative_) {
      allowNegative_ = false;
      
      size_t stop;
      for (stop = 1; isNumber(source[stop]); ++stop); 
      
      token = source.substr(0, stop);
      source = source.substr(stop);
      return token;
    }
    
    if (isOperator(source[0])) {
      token = source[0];
      source = source.substr(1);

      return token;
    } else {
      for (size_t i = 0; i < source.length(); ++i) {
        if (isOperator(source[i]) || source[i] == ' ') {
          token = source.substr(0, i);
          source = source.substr(i);
          allowNegative_ = false;

          return token;
        }
      }
    }

    source = "";
    return token;
  }

  std::vector<std::string> tokenize(std::string& expression) {
    Tokenizer tokenizer;
    std::vector<std::string> tokens;

    for (std::string token = tokenizer.getNextToken(expression); token != ""; token = tokenizer.getNextToken(expression)) {
      tokens.push_back(token);
    }

    return tokens;
  }
}

#endif // MATH_EVAL_HPP
