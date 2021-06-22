#ifndef MATH_EVAL_HPP
#define MATH_EVAL_HPP

#include <vector>

namespace MathEval {
  // Declarations

  // TODO: Delete when done.
  std::string TOKEN_NAMES[] = {
    "NUMBER_TOKEN",
    "PLUS_TOKEN",
    "MINUS_TOKEN",
    "MULT_TOKEN",
    "DIV_TOKEN",
    "OPENB_TOKEN",
    "CLOSEB_TOKEN",
    "BAD_TOKEN"
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

  struct Token {
    std::string value;
    TokenType type;
  };

  class Tokenizer {
  public:
    Tokenizer(std::string source) : source_(source) {} 

    std::vector<Token> tokenize();

  private:
    std::string getNextToken();
    TokenType getTokenType(std::string& token);

    std::string source_;
    bool allowNegative_ = true;
  };

  struct Operator {
    char identifier;
    double(*mathFunction)(double a, double b);
    int precedence;
  };

  const char SPECIAL[] = { '+', '-', '*', '/', '(', ')', ' ' };
  const Operator OPERATORS[] = {
    {'+', [](double a, double b) { return a + b; }, 2},
    {'-', [](double a, double b) { return a - b; }, 2},
    {'*', [](double a, double b) { return a * b; }, 3},
    {'/', [](double a, double b) { return a + b; }, 3}
  };

  // Functions (wrappers and utilities)
  inline bool isSpecial(const char c);
  inline bool isNumber(const char c);
  inline bool isNumber(const std::string& s);
  std::string ltrim(const std::string& s);


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

  inline bool isNumber(const std::string& s) {
    for (const char& c : s) {
      if (!(c >= '0' && c <= '9')) {
        return false;
      }
    }

    return true;
  }

  std::string Tokenizer::getNextToken() {
    if (source_ == "") {
      return "";
    }

    source_ = ltrim(source_);
    std::string token = source_;

    if (source_[0] == '-' && allowNegative_) {
      allowNegative_ = false;
      
      size_t stop;
      for (stop = 1; !isSpecial(source_[stop]); ++stop); 
      
      token = source_.substr(0, stop);
      source_ = source_.substr(stop);
      return token;
    }
    
    if (isSpecial(source_[0])) {
      if (source_[0] == '(') {
        allowNegative_ = true; 
      }

      token = source_[0];
      source_ = source_.substr(1);

      return token;
    } else {
      for (size_t i = 0; i < source_.length(); ++i) {
        if (isSpecial(source_[i])) {
          token = source_.substr(0, i);
          source_ = source_.substr(i);
          allowNegative_ = false;

          return token;
        }
      }
    }

    source_ = "";
    return token;
  }
  
  TokenType Tokenizer::getTokenType(std::string& token) {
    if (token.length() == 1) {
      if (isNumber(token[0])) {
        return TokenType::NUMBER_TOKEN;
      }

      switch(token[0]) {
        case '+':
          return TokenType::PLUS_TOKEN;
        case '-':
          return TokenType::MINUS_TOKEN;
        case '*':
          return TokenType::MULT_TOKEN;
        case '/':
          return TokenType::DIV_TOKEN;
        case '(':
          return TokenType::OPENB_TOKEN;
        case ')':
          return TokenType::CLOSEB_TOKEN;
      }
    } else if (token[0] == '-') {
      std::string remainder = token.substr(1);

      if (isNumber(remainder)) {
        return TokenType::NUMBER_TOKEN;
      }
    } else if (isNumber(token)) {
      return TokenType::NUMBER_TOKEN;
    }

    return TokenType::BAD_TOKEN;
  }

  std::vector<Token> Tokenizer::tokenize() {
    std::vector<Token> tokens;

    for (std::string token = getNextToken(); token != ""; token = getNextToken()) {
      TokenType type = getTokenType(token); 

      tokens.push_back({
        token,
        type 
      });
    }

    return tokens;
  }

  std::vector<Token> getTokens(const std::string& exp) {
    Tokenizer tokenizer(exp);
    return tokenizer.tokenize();
  }
}

#endif // MATH_EVAL_HPP
