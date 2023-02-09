#include "num.h"

Num::Num(int value, int modulo) {
    if (modulo == 0) {
      Num::value = value;
      return;
    }
    Num::value = value % modulo;
    Num::modulo = modulo;
  }
Num::Num(const Num &other) {
    Num::value = other.value;
    Num::modulo = 0;
}

