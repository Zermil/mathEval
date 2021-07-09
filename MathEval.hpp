#ifndef MATH_EVAL_HPP
#define MATH_EVAL_HPP

#include <unordered_map>
#include <cassert>
#include <vector>
#include <cmath>
#include <stack>

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

    Node(Token val) : leaf(val), left(nullptr), right(nullptr) {}
    Node(Token val, Node* pRight, Node* pLeft) : leaf(val), right(pRight), left(pLeft) {}
  };

  // Classes
  class Tokenizer {
  public:
    Tokenizer(const std::string& source) : source_(source) {}

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
    SyntaxTree(const RPN& rpnExp);
    ~SyntaxTree();

    inline Node* getSyntaxTree() const { return ast_; }

  private:
    Node* ast_;
  };

  // Constants, easy to add extra variables, functions or operators
  const std::unordered_map<std::string, double> VARIABLES = {
    { "pi",  atan(1) * 4 },
    { "e" ,  exp(1)      },
    { "rc",  1729        }
  };

  const std::unordered_map<char, OperatorExpr> OPERATORS = {
    { '+', { [](double a, double b) { return a + b;      }, 2, true  } },
    { '-', { [](double a, double b) { return a - b;      }, 2, true  } },
    { '*', { [](double a, double b) { return a * b;      }, 3, true  } },
    { '/', { [](double a, double b) { return a / b;      }, 3, true  } },
    { '%', { [](double a, double b) { return fmod(a, b); }, 3, true  } },
    { '^', { [](double a, double b) { return pow(a, b);  }, 4, false } }
  };

  const std::unordered_map<std::string, pUnaryFunction> UNARY_FUNCTIONS = {
    { "sin",  [](double a) { return sin(a);  } },
    { "cos",  [](double a) { return cos(a);  } },
    { "sqrt", [](double a) { return sqrt(a); } }
  };

  const std::unordered_map<std::string, pBinaryFunction> BINARY_FUNCTIONS = {
    { "max", [](double a, double b) { return a > b ? a : b; } }
  };
  
  // Extra special characters that are checked alongside operators
  const char SPECIAL[] = { '(', ')', ' ', ',' };

  // Functions (wrappers & utilities)
  bool isSpecial(const char& c);
  bool isOperator(const char& c);
  bool isVariable(const std::string& s);
  bool isNumber(const std::string& s);
  bool isUnaryFunction(const std::string& s);
  bool isBinaryFunction(const std::string& s);
  bool isFunction(const std::string& s);
  std::string ltrim(const std::string& s);
  std::string toLower(const std::string& s);
  void deleteAST(Node* ast);

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

  bool isSpecial(const char& c)
  {
    for (auto it = OPERATORS.begin(); it != OPERATORS.end(); ++it) {
      if (it->first == c) {
        return true;
      }
    }

    for (const char& special : SPECIAL) {
      if (special == c) {
        return true;
      }
    }

    return false;
  }

  bool isOperator(const char& c)
  {
    if (OPERATORS.find(c) == OPERATORS.end()) {
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

  void deleteAST(Node* ast)
  {
    if (ast == nullptr) return; 

    deleteAST(ast->left);
    deleteAST(ast->right);

    delete ast;
    ast = nullptr;
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

    if (token.length() == 1 && isOperator(token[0])) {
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
            int topPrecedence = OPERATORS.at(operatorStack.top().value[0]).precedence;
            int tokenPrecedence = OPERATORS.at(token.value[0]).precedence;
            bool isLeftAssociative = OPERATORS.at(token.value[0]).leftAssociative;

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
  SyntaxTree::SyntaxTree(const RPN& rpnExp)
  {
    std::stack<Node*> expressions;

    for (const Token& token : rpnExp) {
      if (token.type == TokenType::NUMBER_TOKEN || token.type == TokenType::VARIABLE_TOKEN) {
        expressions.push(new Node(token));

        continue;
      }

      if (token.type == TokenType::OPERATOR_TOKEN) {
        assert(("Incomplete/Invalid expression", expressions.size() >= 2));
        Node* right = expressions.top();
        expressions.pop();

        Node* left = expressions.top();
        expressions.pop();

        expressions.push(new Node(token, right, left));
        
        continue;
      }

      if (token.type == TokenType::FUNCTION_TOKEN) {
        if (isBinaryFunction(token.value) || isBinaryFunction(token.value.substr(1))) {
          assert(("Incomplete/Invalid expression", expressions.size() >= 2));
          Node* right = expressions.top();
          expressions.pop();

          Node* left = expressions.top();
          expressions.pop();

          expressions.push(new Node(token, right, left));
        } else {
          assert(("Incomplete/Invalid expression", expressions.size() >= 1));
          Node* left = expressions.top();
          expressions.pop();

          expressions.push(new Node(token, nullptr, left));
        }

        continue;
      }
    }

    assert(("Incomplete/Invalid expression", expressions.size() == 1));

    ast_ = expressions.top();
    expressions = std::stack<Node*>();
  }

  SyntaxTree::~SyntaxTree() 
  {
    deleteAST(ast_);
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
        output = OPERATORS.at(leafValue[0]).mathFunction(evalSyntaxTree(tree->left), evalSyntaxTree(tree->right));
        break;
      case TokenType::FUNCTION_TOKEN:
        if (isBinaryFunction(leafValue)) output = BINARY_FUNCTIONS.at(leafValue)(evalSyntaxTree(tree->left), evalSyntaxTree(tree->right));
        else output = UNARY_FUNCTIONS.at(leafValue)(evalSyntaxTree(tree->left));
        break;
    }

    return isNegative ? -output : output;
  }

  double evalExpression(std::string expression)
  {
    Tokenizer tokenizer(toLower(expression));
    RPN rpnExp = tokenizer.buildRPN();

    SyntaxTree tree(rpnExp);
    Node* ast = tree.getSyntaxTree();

    return evalSyntaxTree(ast);
  }
}

#endif // MATH_EVAL_HPP
