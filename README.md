yourhtml lexer
=============

A hand rolled lexer implemented as described in [HTML Living Standard](https://html.spec.whatwg.org/multipage/parsing.html#tokenization) Tokenization.

## Building

**Dependencies**

- [clang](https://llvm.org) - c++ compiler
- [ib](https://github.com/JasonL9000/ib) - c++ build tool

```bash
git submodule init
git submodule update

# run all tests
ib --test_all test

# build a single test
ib test/simple_token-test

# run
../out/debug/test/selector-test
```

## Example

```c++
int main(int, char*[]) {
  // TODO
  return 0;
}
```
