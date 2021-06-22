#include <iostream>
#include "MathEval.hpp"

int main(int argc, char* argv[])
{
  std::string EXPRESSIONS[] = {
    "   123 +   12 * 7   ",
    "1 + 2 * 3",
    "(-1 + 2) * 3",
    "2 * 3 + 1",
    "-123 + 4 - 16 +(-3-4)-6",
    "-123 - 3",
    "123- 3",
    "123-3"
  };

  for (std::string& exp : EXPRESSIONS) {
    std::vector<MathEval::Token> tokens = MathEval::getTokens(exp);
    
    std::cout << "EXPRESSION: " << exp << std::endl;

    for (const MathEval::Token& token : tokens) {
      std::cout << token.value << ": " << MathEval::TOKEN_NAMES[static_cast<int>(token.type)] << std::endl; 
    }

    std::cout << "===================" << std::endl;
  }

  system("pause");
  return 0;
}
