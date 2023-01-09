#pragma once

class Num {
 public:
  Num(int value, int modulo);

  Num& operator=(const Num& other);

  Num operator+(const Num& other) const;
  Num operator-(const Num& other) const;
  Num operator*(const Num& other) const;
  Num operator+(int num) const;
  Num operator-(int num) const;
  Num operator*(int num) const;

  Num& operator+=(const Num& other);
  Num& operator-=(const Num& other);
  Num& operator*=(const Num& other);
  Num& operator+=(int num);
  Num& operator-=(int num);
  Num& operator*=(int num);

 private:
  int value_;
  int modulo_;
};