#ifndef MATH_EVAL_HPP
#define MATH_EVAL_HPP

#include <stdexcept>
#include <algorithm>
#include <iostream>
#include <vector>
#include <cmath>
#include <stack>
#include <map>

namespace MathEval {
  //
  // Declarations
  //
  typedef double(*pUnaryFunction)(double a);
  typedef double(*pBinaryFunction)(double a, double b);

  enum class TokenType {
    NUMBER_TOKEN = 0,
    VARIABLE_TOKEN,
    OPERATOR_TOKEN,
    OPENB_TOKEN,
    CLOSEB_TOKEN,
    FUNCTION_TOKEN,
    BAD_TOKEN,
  };

  struct Token {
    std::string value;
    TokenType type;
  };

  struct OperatorExpr {
    pBinaryFunction mathFunction;
    int precedence;
    bool leftAssociative;
  };

  class Tokenizer {
  public:
    Tokenizer(std::string source) : source_(source), expr_(source) {} 

    std::vector<Token> getRPN();

  private:
    std::string getNextToken();
    TokenType getTokenType(std::string& token) const;
    std::vector<Token> getAllTokens();
    std::vector<Token> buildRPN();

    std::string source_;
    std::string expr_; // Just for better error throwing
    bool allowNegative_ = true;
  };

  const char SPECIAL[] = { '+', '-', '*', '/', '(', ')', ' ', ',', '^', '%' };

  const std::map<std::string, double> VARIABLES = {
    { "pi",  3.141592653 },
    { "e" ,  2.718281828 },
    { "rc",  1729        }
  };

  const std::map<std::string, OperatorExpr> OPERATORS = {
    { "+", { [](double a, double b) { return a + b;      }, 2, true  } },
    { "-", { [](double a, double b) { return a - b;      }, 2, true  } },
    { "*", { [](double a, double b) { return a * b;      }, 3, true  } },
    { "/", { [](double a, double b) { return a / b;      }, 3, true  } },
    { "%", { [](double a, double b) { return fmod(a, b); }, 3, true  } },
    { "^", { [](double a, double b) { return pow(a, b);  }, 4, false } }
  };

  const std::map<std::string, pUnaryFunction> UNARY_FUNCTIONS = {
    { "sin", [](double a) { return sin(a); } },
    { "cos", [](double a) { return cos(a); } }
  };

  const std::map<std::string, pBinaryFunction> BINARY_FUNCTIONS = {
    { "max", [](double a, double b) { return std::max(a, b); } }
  };

  // Functions (wrappers & utilities)
  bool isSpecial(const char c);
  bool isOperator(const std::string& s);
  bool isVariable(const std::string& s);
  bool isNumber(const std::string& s);
  bool isUnaryFunction(const std::string& s);
  bool isBinaryFunction(const std::string& s);
  bool isFunction(const std::string& s);
  std::string ltrim(const std::string& s);
  std::string toLower(const std::string& s);

  
  //
  // Implementations
  //
  std::string ltrim(const std::string& s) 
  {
    size_t start;
    for (start = 0; s[start] == ' '; ++start);
    return s.substr(start);
  }

  std::string toLower(const std::string& s) 
  {
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

  bool isSpecial(const char c) 
  {
    for (const char& special : SPECIAL) {
      if (special == c) {
        return true;
      }
    }

    return false;
  }

  bool isOperator(const std::string& s) 
  {
    if (OPERATORS.find(s) == OPERATORS.end()) {
      return false;
    }

    return true;
  }

  bool isUnaryFunction(const std::string& s) 
  {
    if (UNARY_FUNCTIONS.find(toLower(s)) == UNARY_FUNCTIONS.end()) {
      return false;
    }

    return true;
  }

  bool isBinaryFunction(const std::string& s)
  {
    if (BINARY_FUNCTIONS.find(toLower(s)) == BINARY_FUNCTIONS.end()) {
      return false;
    }

    return true;
  }

  bool isFunction(const std::string& s) 
  {
    if (s[0] == '-') {
      return isBinaryFunction(s.substr(1)) || isUnaryFunction(s.substr(1));
    }

    return isBinaryFunction(s) || isUnaryFunction(s);
  }

  bool isVariable(const std::string& s) 
  {
    if (VARIABLES.find(toLower(s)) == VARIABLES.end()) {
      return false;
    }

    return true;
  }

  bool isNumber(const std::string& s) 
  {
    char* endPtr = nullptr;
    double number = strtod(s.c_str(), &endPtr);

    return endPtr != s.c_str() && *endPtr == '\0' && number != HUGE_VAL;
  }


  // 
  // Tokenizer Implementations
  //
  std::vector<Token> Tokenizer::getRPN() 
  {
    std::vector<Token> RPNexpr;

    try {
      RPNexpr = buildRPN();
    } catch (const std::runtime_error& error) {
      std::cout << error.what() << '\n'; 
    }

    return RPNexpr;
  }
  
  std::string Tokenizer::getNextToken() 
  {
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
  
  TokenType Tokenizer::getTokenType(std::string& token) const 
  {
    if (isFunction(token)) {
      return TokenType::FUNCTION_TOKEN;
    }

    if (isNumber(token)) {
      return TokenType::NUMBER_TOKEN;
    }

    if (isVariable(token)) {
      return TokenType::VARIABLE_TOKEN;
    }

    if (token.length() == 1) {
      if (isOperator(token)) {
        return TokenType::OPERATOR_TOKEN;
      }

      switch (token[0]) {
        case '(': return TokenType::OPENB_TOKEN;
        case ')': return TokenType::CLOSEB_TOKEN;
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

  std::vector<Token> Tokenizer::getAllTokens() 
  {
    std::vector<Token> tokens;

    for (std::string token = getNextToken(); token != ""; token = getNextToken()) {
      // Ignore commas
      if (token == ",") {
        continue;
      }

      TokenType type = getTokenType(token); 
      tokens.push_back({ token, type });
    }

    return tokens;
  }

  std::vector<Token> Tokenizer::buildRPN() 
  {
    std::vector<Token> tokens = getAllTokens();

    std::stack<Token> operatorStack;
    std::vector<Token> exprQueue; 
    
    for (const Token& token : tokens) {
      if (token.type == TokenType::BAD_TOKEN) {
        throw std::runtime_error("Unrecognized token in: " + expr_);
      }

      if (token.type == TokenType::OPENB_TOKEN || token.type == TokenType::FUNCTION_TOKEN) {
        operatorStack.push(token);
        continue;
      }

      if (token.type == TokenType::NUMBER_TOKEN || token.type == TokenType::VARIABLE_TOKEN) {
        exprQueue.push_back(token);
        continue;
      }

      if (token.type == TokenType::OPERATOR_TOKEN) {
        while (!operatorStack.empty() && operatorStack.top().type != TokenType::OPENB_TOKEN) { 

          if (operatorStack.top().type == TokenType::OPERATOR_TOKEN) {
            int topPrecedence = OPERATORS.at(operatorStack.top().value).precedence;
            int tokenPrecedence = OPERATORS.at(token.value).precedence; 
            bool isLeftAssociative = OPERATORS.at(token.value).leftAssociative;
            
            if (topPrecedence >= tokenPrecedence && isLeftAssociative) {
              exprQueue.push_back(operatorStack.top());
              operatorStack.pop();
            } else {
              break;
            }
          } else {
            exprQueue.push_back(operatorStack.top());
            operatorStack.pop();
          }

        }

        operatorStack.push(token);
        continue;
      }

      if (token.type == TokenType::CLOSEB_TOKEN) {
        while (!operatorStack.empty() && operatorStack.top().type != TokenType::OPENB_TOKEN) {
          exprQueue.push_back(operatorStack.top());
          operatorStack.pop();
        }

        if (operatorStack.empty()) {
          throw std::runtime_error("Mismatched parenthesis in: " + expr_);
        }
        
        operatorStack.pop();
        continue;
      }
    }
    
    // Dequeue remainder
    while (!operatorStack.empty()) {
      if (operatorStack.top().type == TokenType::OPENB_TOKEN) {
        throw std::runtime_error("Mismatched parenthesis in: " + expr_);
      }

      exprQueue.push_back(operatorStack.top());
      operatorStack.pop();
    }

    return exprQueue; 
  }
}

#endif // MATH_EVAL_HPP
