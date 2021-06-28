#ifndef MATH_EVAL_HPP
#define MATH_EVAL_HPP

#include <cassert>
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
  typedef std::vector<Token> RPN;

  struct OperatorExpr {
    pBinaryFunction mathFunction;
    int precedence;
    bool leftAssociative;
  };

  struct Node {
    Token leaf;
    Node* left;
    Node* right;
  };

  // Classes
  class Tokenizer {
  public:
    Tokenizer(std::string source) : source_(source) {}

    RPN buildRPN();

  private:
    TokenType getTokenType(const std::string& token) const;
    std::string getNextToken();
    RPN getRpnTokens();

    std::string source_;
    bool allowNegative_ = true;
  };

  class SyntaxTree {
  public:
    SyntaxTree(RPN rpnExp) : rpnExp_(rpnExp) {}

    Node* buildSyntaxTree() const;

  private:
    RPN rpnExp_;
  };

  // Constants
  const char SPECIAL[] = { '+', '-', '*', '/', '(', ')', ' ', ',', '^', '%' };

  const std::map<std::string, double> VARIABLES = {
    { "pi",  atan(1) * 4 },
    { "e" ,  exp(1)      },
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
    { "max", [](double a, double b) { return a > b ? a : b; } }
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

  double evalSyntaxTree(Node* tree);
  double evalExpression(std::string expression);


  //
  // Helper implementations
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
    if (UNARY_FUNCTIONS.find(s) == UNARY_FUNCTIONS.end()) {
      return false;
    }

    return true;
  }

  bool isBinaryFunction(const std::string& s)
  {
    if (BINARY_FUNCTIONS.find(s) == BINARY_FUNCTIONS.end()) {
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
    std::string value = s;
    if (s[0] == '-') value = value.substr(1);

    if (VARIABLES.find(value) == VARIABLES.end()) {
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
  // Tokenizer implementations
  //
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
      for (stop = 1; !isSpecial(source_[stop]) && source_[stop] != '\0'; ++stop);

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
    }
    else {
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

  TokenType Tokenizer::getTokenType(const std::string& token) const
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

    if (isOperator(token)) {
      return TokenType::OPERATOR_TOKEN;
    }

    if (token == "(") return TokenType::OPENB_TOKEN;
    if (token == ")") return TokenType::CLOSEB_TOKEN;

    return TokenType::BAD_TOKEN;
  }

  RPN Tokenizer::getRpnTokens()
  {
    RPN tokens;

    for (std::string token = getNextToken(); token != ""; token = getNextToken()) {
      // Ignore commas
      if (token == ",") {
        continue;
      }

      TokenType type = getTokenType(token);

      assert(("Unrecognized token", type != TokenType::BAD_TOKEN));
      tokens.push_back({ token, type });
    }

    return tokens;
  }

  RPN Tokenizer::buildRPN()
  {
    RPN tokens = getRpnTokens();

    std::stack<Token> operatorStack;
    RPN exprQueue;

    for (const Token& token : tokens) {
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
            }
            else {
              break;
            }
          }
          else {
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

        assert(("Mismatched parenthesis", !operatorStack.empty()));

        operatorStack.pop();
        continue;
      }
    }

    // Dequeue remainder
    while (!operatorStack.empty()) {
      assert(("Mismatched parenthesis", operatorStack.top().type != TokenType::OPENB_TOKEN));

      exprQueue.push_back(operatorStack.top());
      operatorStack.pop();
    }

    return exprQueue;
  }


  //
  // SyntaxTree implementations
  //
  Node* SyntaxTree::buildSyntaxTree() const
  {
    std::stack<Node*> expressions;

    for (const Token& token : rpnExp_) {
      if (token.type == TokenType::NUMBER_TOKEN || token.type == TokenType::VARIABLE_TOKEN) {
        Node* numNode = new Node;

        numNode->leaf = token;
        numNode->left = nullptr;
        numNode->right = nullptr;

        expressions.push(numNode);
        continue;
      }

      if (token.type == TokenType::OPERATOR_TOKEN) {
        assert(("Incomplete/Invalid expression", expressions.size() >= 2));
        Node* opNode = new Node;

        opNode->leaf = token;

        opNode->right = expressions.top();
        expressions.pop();

        opNode->left = expressions.top();
        expressions.pop();

        expressions.push(opNode);
        continue;
      }

      if (token.type == TokenType::FUNCTION_TOKEN) {
        Node* funcNode = new Node;

        if (isBinaryFunction(token.value) || isBinaryFunction(token.value.substr(1))) {
          assert(("Incomplete/Invalid expression", expressions.size() >= 2));

          funcNode->leaf = token;

          funcNode->right = expressions.top();
          expressions.pop();

          funcNode->left = expressions.top();
          expressions.pop();
        }
        else {
          assert(("Incomplete/Invalid expression", expressions.size() >= 1));

          funcNode->leaf = token;
          funcNode->right = nullptr;

          funcNode->left = expressions.top();
          expressions.pop();
        }

        expressions.push(funcNode);
        continue;
      }
    }

    assert(("Incomplete/Invalid expression", expressions.size() == 1));
    return expressions.top();
  }


  //
  // Wrapper functions implementations
  //
  double evalSyntaxTree(Node* tree)
  {
    std::string leafValue = tree->leaf.value;
    TokenType type = tree->leaf.type;

    bool isNegative = leafValue.length() > 1 && leafValue[0] == '-';
    if (isNegative) leafValue = leafValue.substr(1);

    double output;

    switch (type) {
      case TokenType::NUMBER_TOKEN:
        output = std::strtod(leafValue.c_str(), nullptr);
        break;
      case TokenType::VARIABLE_TOKEN:
        output = VARIABLES.at(leafValue);
        break;
      case TokenType::OPERATOR_TOKEN:
        output = OPERATORS.at(leafValue).mathFunction(evalSyntaxTree(tree->left), evalSyntaxTree(tree->right));
        break;
      case TokenType::FUNCTION_TOKEN:
        if (isBinaryFunction(leafValue)) output = BINARY_FUNCTIONS.at(leafValue)(evalSyntaxTree(tree->left), evalSyntaxTree(tree->right));
        else output = UNARY_FUNCTIONS.at(leafValue)(evalSyntaxTree(tree->left));
        break;
    }

    delete tree; // Node no longer needed free memory
    return isNegative ? -output : output;
  }

  double evalExpression(std::string expression)
  {
    Tokenizer tokenizer(toLower(expression));
    RPN rpnExp = tokenizer.buildRPN();

    SyntaxTree tree(rpnExp);
    Node* ast = tree.buildSyntaxTree();

    return evalSyntaxTree(ast);
  }
}

#endif // MATH_EVAL_HPP