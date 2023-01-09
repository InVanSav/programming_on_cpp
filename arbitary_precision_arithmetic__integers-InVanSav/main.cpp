#include <iostream>
#include <Integer.hpp>
#include <cassert>

int main() {
  {
    arbitary_precision_arithmetic::Integer a(1000000000000000001);
    arbitary_precision_arithmetic::Integer b(99);
    arbitary_precision_arithmetic::Integer c(a + b);
    arbitary_precision_arithmetic::Integer d(a + b);
    assert(c == d);
    std::cout << c << std::endl;

    arbitary_precision_arithmetic::Integer g(1000000000000000001);
    arbitary_precision_arithmetic::Integer h(99);
    arbitary_precision_arithmetic::Integer e(a += b);
    arbitary_precision_arithmetic::Integer f(g += h);
    assert(e == f);
    std::cout << e << std::endl;
  }
  {
    arbitary_precision_arithmetic::Integer a(-1000000000000000001);
    arbitary_precision_arithmetic::Integer b(99);
    arbitary_precision_arithmetic::Integer c(a + b);
    arbitary_precision_arithmetic::Integer d(a + b);
    assert(c == d);
    std::cout << c << std::endl;
  }
  {
    arbitary_precision_arithmetic::Integer a(99);
    arbitary_precision_arithmetic::Integer b(99);
    arbitary_precision_arithmetic::Integer c(a++);
    arbitary_precision_arithmetic::Integer d(b++);
    assert(c == d);
    std::cout << c << std::endl;
  }
  {
    arbitary_precision_arithmetic::Integer a(1000000000000000100);
    arbitary_precision_arithmetic::Integer b(99);
    arbitary_precision_arithmetic::Integer c(a - b);
    arbitary_precision_arithmetic::Integer d(a - b);
    assert(c == d);
    std::cout << c << std::endl;

    arbitary_precision_arithmetic::Integer e(a -= b);
    arbitary_precision_arithmetic::Integer f(a -= b);
    assert(e == f);
    std::cout << e << std::endl;
  }
  {
    arbitary_precision_arithmetic::Integer a(1000000000000000001);
    arbitary_precision_arithmetic::Integer b(99);
    arbitary_precision_arithmetic::Integer c(a * b);
    arbitary_precision_arithmetic::Integer d(a * b);
    assert(c == d);
    std::cout << c << std::endl;

    arbitary_precision_arithmetic::Integer g(1000000000000000001);
    arbitary_precision_arithmetic::Integer h(99);
    arbitary_precision_arithmetic::Integer e(a *= b);
    arbitary_precision_arithmetic::Integer f(g *= h);
    assert(e == f);
    std::cout << e << std::endl;
  }
  {
    arbitary_precision_arithmetic::Integer a(1000000000000000001);
    arbitary_precision_arithmetic::Integer b(99);
    arbitary_precision_arithmetic::Integer c(a / b);
    arbitary_precision_arithmetic::Integer d(a / b);
    assert(c == d);
    std::cout << c << std::endl;

    arbitary_precision_arithmetic::Integer e(a /= b);
    arbitary_precision_arithmetic::Integer f(a /= b);
    assert(e == f);
    std::cout << e << std::endl;
  }
  {
    arbitary_precision_arithmetic::Integer a(1000000000000000001);
    arbitary_precision_arithmetic::Integer b(99);
    arbitary_precision_arithmetic::Integer c(a % b);
    arbitary_precision_arithmetic::Integer d(a % b);
    assert(c == d);
    std::cout << c << std::endl;

    arbitary_precision_arithmetic::Integer e(a %= b);
    arbitary_precision_arithmetic::Integer f(a %= b);
    assert(e == f);
    std::cout << e << std::endl;
  }
  {
    arbitary_precision_arithmetic::Integer a(1000000000000000001);
    arbitary_precision_arithmetic::Integer b(99);
    bool flag = a < b;
    assert(a < b);
    std::cout << flag << std::endl;
    flag = a <= b;
    assert(a <= b);
    std::cout << flag << std::endl;
    flag = a > b;
    assert(!(a > b));
    std::cout << flag << std::endl;
    flag = a >= b;
    assert(!(a >= b));
    std::cout << flag << std::endl;
    flag = a == b;
    assert(!(a == b));
    std::cout << flag << std::endl;
  }

  std::cout << 1;
  return 0;
}
