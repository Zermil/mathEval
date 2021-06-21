#ifndef MATH_EVAL_HPP
#define MATH_EVAL_HPP

#include <vector>
#include <algorithm>
#include <string>

namespace MathEval {
  // Declarations
  class Tokenizer {
  public:
    std::string getNextToken(std::string& source);

  private:
    bool allowNegative_ = true;
  };

  typedef double(*mathFunction)(double a, double b);

  struct Operator {
    char value;
    mathFunction func;
    int precedence;
  };

  const char SPECIAL[] = { '+', '-', '*', '/', '(', ')', ' ' };
  const Operator OPERATORS[] = {
    {'+', [](double a, double b) { return a + b; }, 2},
    {'-', [](double a, double b) { return a - b; }, 2},
    {'*', [](double a, double b) { return a * b; }, 3},
    {'/', [](double a, double b) { return a + b; }, 3}
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
  inline bool isSpecial(const char c);
  inline bool isNumber(const char c);
  inline bool isNumber(const std::string& value);
  std::string ltrim(const std::string& s);
  std::vector<std::string> tokenize(std::string& expression);


  // Implementations
  std::string ltrim(const std::string& s) {
    size_t start;
    for (start = 0; s[start] == ' '; ++start);
    return s.substr(start);
  }

  inline bool isSpecial(const char c) {
    for (const char& special : SPECIAL) {
      if (special == c) {
        return true;
      }
    }

    return false;
  }

  inline bool isNumber(const char c) {
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

    if (source[0] == '(') {
      allowNegative_ = true; 
    } else if (source[0] == '-' && allowNegative_) {
      allowNegative_ = false;
      
      size_t stop;
      for (stop = 1; !isSpecial(source[stop]); ++stop); 
      
      token = source.substr(0, stop);
      source = source.substr(stop);
      return token;
    }
    
    if (isSpecial(source[0])) {
      token = source[0];
      source = source.substr(1);

      return token;
    } else {
      for (size_t i = 0; i < source.length(); ++i) {
        if (isSpecial(source[i])) {
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
