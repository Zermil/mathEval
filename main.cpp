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
    "-pI + 3 - 4 + (-3 + 2)-E*3",
    "4.+3-e*3+(-3)",
    "-123+(-3)",
    "-3-"
  };

  for (std::string& exp : EXPRESSIONS) {
    MathEval::Tokenizer tokenizer(exp);
    std::vector<MathEval::Token> rpnExp = tokenizer.getRPN();
    
    std::cout << "EXPRESSION: " << exp << std::endl;

    for (const MathEval::Token& rpn : rpnExp) {
      std::cout << rpn.value << ": " << static_cast<int>(rpn.type) << std::endl; 
    }

    std::cout << "===================" << std::endl;
  }

  system("pause");
  return 0;
}
