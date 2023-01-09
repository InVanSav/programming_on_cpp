#include "num.h"
#include <cstdint>

Num::Num(int value, int modulo) {
  if (modulo == 0) {
    this->value_ = value;
    return;
  }
  value_ = value % modulo;
  modulo_ = modulo;
}

Num& Num::operator=(const Num& other) = default;

Num Num::operator+(const Num& other) const {
  return {Num::value + other.value, Num::modulo};
}

Num Num::operator-(const Num& other) const {
  int val = Num::value - other.value, mod = Num::modulo;
  if (val < 0) {
    val = (val % mod + mod) % mod;
  }
  return {val, mod};
}

Num Num::operator*(const Num& other) const {
  return {Num::value * other.value, Num::modulo};
}

Num Num::operator+(int num) const {
  return {Num::value + num, Num::modulo};
}

Num Num::operator-(int num) const {
  int val = Num::value - num, mod = Num::modulo;
  if (val < 0) {
    val = (val % mod + mod) % mod;
  }
  return {val, mod};
}

Num Num::operator*(int num) const {
  return {Num::value * num, Num::modulo};
}

Num& Num::operator+=(const Num& other) {
  if (Num::modulo == 0) {
    Num::value += other.value;
    return *this;
  }
  Num::value = static_cast<int>(
      (static_cast<int64_t>(Num::value) + static_cast<int64_t>(other.value)) %
          static_cast<int64_t>(Num::modulo));
  return *this;
}

Num& Num::operator-=(const Num& other) {
  if (Num::modulo == 0) {
    Num::value -= other.value;
    return *this;
  }
  Num::value = static_cast<int>(
      (static_cast<int64_t>(Num::value) - static_cast<int64_t>(other.value)) %
          static_cast<int64_t>(Num::modulo));

  if (Num::value < 0) {
    Num::value = (Num::value % Num::modulo + Num::modulo) % Num::modulo;
  }
  return *this;
}

Num& Num::operator*=(const Num& other) {
  if (Num::modulo == 0) {
    Num::value *= other.value;
    return *this;
  }
  Num::value = static_cast<int>(
      (static_cast<int64_t>(Num::value) * static_cast<int64_t>(other.value)) %
          static_cast<int64_t>(Num::modulo));
  return *this;
}

Num& Num::operator+=(int num) {
  if (Num::modulo == 0) {
    Num::value += num;
    return *this;
  }
  Num::value = static_cast<int>(
      (static_cast<int64_t>(Num::value) + static_cast<int64_t>(num)) %
          static_cast<int64_t>(Num::modulo));
  return *this;
}

Num& Num::operator-=(int num) {
  if (Num::modulo == 0) {
    Num::value -= num;
    return *this;
  }
  Num::value = static_cast<int>(
      (static_cast<int64_t>(Num::value) - static_cast<int64_t>(num)) %
          static_cast<int64_t>(Num::modulo));

  if (Num::value < 0) {
    Num::value = (Num::value % Num::modulo + Num::modulo) % Num::modulo;
  }

  return *this;
}

Num& Num::operator*=(int num) {
  if (Num::modulo == 0) {
    Num::value *= num;
    return *this;
  }
  Num::value = static_cast<int>(
      (static_cast<int64_t>(Num::value) * static_cast<int64_t>(num)) %
          static_cast<int64_t>(Num::modulo));

  return *this;
}
