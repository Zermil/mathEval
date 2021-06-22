#ifndef MATH_EVAL_HPP
#define MATH_EVAL_HPP

#include <vector>

namespace MathEval {
  // Declarations

  // TODO: Delete when done.
  std::string TOKEN_NAMES[] = {
    "NUMBER_TOKEN",
    "VARIABLE_TOKEN",
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
    VARIABLE_TOKEN,
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

  struct Operator {
    char identifier;
    double(*mathFunction)(double a, double b);
    int precedence;
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

  const char SPECIAL[] = { '+', '-', '*', '/', '(', ')', ' ' };
  const std::string VARIABLES[] = { "pi", "e" };
  const Operator OPERATORS[] = {
    {'+', [](double a, double b) { return a + b; }, 2},
    {'-', [](double a, double b) { return a - b; }, 2},
    {'*', [](double a, double b) { return a * b; }, 3},
    {'/', [](double a, double b) { return a + b; }, 3}
  };

  // Functions (wrappers & utilities)
  bool isSpecial(const char c);
  bool isVariable(const std::string& s);
  bool isNumber(const std::string& s);
  std::string ltrim(const std::string& s);
  std::string toLower(const std::string& s);

  std::vector<Token> getTokens(const std::string& exp); 


  // Implementations
  std::string ltrim(const std::string& s) {
    size_t start;
    for (start = 0; s[start] == ' '; ++start);
    return s.substr(start);
  }

  std::string toLower(const std::string& s) {
    std::string lowered = "";

    for (const char& c : s) {
      if (c >= 'A' && c <= 'Z') {
        lowered += (c - 'A') + 'a';
        continue;
      }

      lowered += c;
    }

    return lowered;
  }

  bool isSpecial(const char c) {
    for (const char& special : SPECIAL) {
      if (special == c) {
        return true;
      }
    }

    return false;
  }

  bool isNumber(const std::string& s) {
    for (const char& c : s) {
      if (!(c >= '0' && c <= '9')) {
        return false;
      }
    }

    return true;
  }

  bool isVariable(const std::string& s) {
    for (const std::string& variable : VARIABLES) {
      if (variable == toLower(s)) {
        return true;
      }
    }

    return false;
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
    if (isNumber(token)) {
      return TokenType::NUMBER_TOKEN;
    }

    if (isVariable(token)) {
      return TokenType::VARIABLE_TOKEN;
    }

    if (token.length() == 1) {
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
    } 

    if (token[0] == '-') {
      std::string remainder = token.substr(1);

      if (isNumber(remainder)) {
        return TokenType::NUMBER_TOKEN;
      } 

      if (isVariable(remainder)) {
        return TokenType::VARIABLE_TOKEN;
      }
    } 

    return TokenType::BAD_TOKEN;
  }

  std::vector<Token> Tokenizer::tokenize() {
    std::vector<Token> tokens;

    for (std::string token = getNextToken(); token != ""; token = getNextToken()) {
      TokenType type = getTokenType(token); 
      tokens.push_back({ token, type });
    }

    return tokens;
  }

  std::vector<Token> getTokens(const std::string& exp) {
    Tokenizer tokenizer(exp);
    return tokenizer.tokenize();
  }
}

#endif // MATH_EVAL_HPP
