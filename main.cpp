#include <iostream>
#include "MathEval.hpp"

int main(int argc, char* argv[])
{
  std::string EXPRESSIONS[] = {
    "   123 +   12 * 7   ",
    "1 + 2 * 3",
    "2 * 3 + 1",
    "-123 + 3",
    "-123 - 3"
  };

  for (std::string& exp : EXPRESSIONS) {
    std::cout << "EXPRESSION: " << exp << std::endl;
    std::vector<std::string> tokens = MathEval::tokenize(exp);

    for (const std::string& value : tokens) {
      std::cout << value << std::endl; 
    }

    std::cout << "===================" << std::endl;
  }

  system("pause");
  return 0;
}
