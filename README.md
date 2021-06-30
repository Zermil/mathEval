# MathEval
Single header "style" C++ library for evaluating simple mathematical expressions, allows for functions (positive and negative) `(max, cos, sin)` and constant variables `(pi, e, rc)` as well as negative and decimal numbers.

## Quick start
```cpp
#include <iostream>
#include "MathEval.hpp"

int main(int argc, char* argv[])
{
  double exprValue = MathEval::evalExpression("1+2*3");
  std::cout << exprValue << '\n';

  return 0;
}
```
### `Build:`
```console
> g++ main.cpp
```
### `Run:`
```console
> a.exe
```
**Note:** Make sure negative function expressions are written as Ex. -> `"-cos(2.1)"` **NO SPACES!** between `'-'` and function expression, otherwise Tokenizer will treat it as "minus" sign/operator, same goes for negative numbers.

## See also
- [asl-calculator](https://github.com/aslze/asl-calculator)
- [MathExpr](https://github.com/k3a/MathExpr)
- [mathpresso](https://github.com/tartakynov/mathpresso)
