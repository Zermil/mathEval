# MathEval
Single header "style" C++ library for evaluating simple mathematical expressions, allows for functions (positive and negative) `(max, cos, sin)` and constant variables `(pi, e, rc)` as well as negative and decimal numbers.

## Quick start
### `Build:`
```console
> g++ main.cpp
```
### `Run:`
```console
> a.exe
```
**Note:** Make sure negative function expression is written as Ex. -> `"-cos(2.1)"` **NO SPACE!** between minus and function expression, otherwise Tokenizer will treat it as "minus" sign/operator, same goes for negative numbers.