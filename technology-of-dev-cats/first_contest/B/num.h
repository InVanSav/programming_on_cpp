#pragma once

#include <iostream>

class Num {
 public:
  Num(int value=0, int modulo=0);
  Num(const Num& other);

  __attribute__((unused)) int value_;
  int modulo;
};
