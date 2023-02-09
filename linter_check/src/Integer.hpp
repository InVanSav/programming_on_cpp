#pragma once
#include <iostream>
#include <vector>

namespace arbitary_precision_arithmetic {
class Integer final {
 public:
  Integer();
  Integer(const Integer&);
  Integer(Integer&&);
  Integer(const std::vector<unsigned int>&);

  Integer& operator=(const Integer&);
  Integer& operator=(Integer&&) noexcept;

  explicit Integer(unsigned char);
  explicit Integer(char);

  explicit Integer(unsigned short);
  explicit Integer(short);

  explicit Integer(unsigned int);
  explicit Integer(int);

  explicit Integer(long unsigned);
  explicit Integer(long);

  explicit Integer(long long unsigned);
  explicit Integer(long long);

  ~Integer();

  Integer operator-() const;
  Integer& operator--();
  Integer operator--(int);

  Integer operator+() const;
  Integer& operator++();
  Integer operator++(int);

  [[nodiscard]] Integer operator-(const Integer&) const;
  Integer& operator-=(const Integer&);

  [[nodiscard]] Integer operator+(const Integer&) const;
  Integer& operator+=(const Integer&);

  [[nodiscard]] Integer operator*(const Integer&) const;
  Integer& operator*=(const Integer&);

  [[nodiscard]] Integer operator/(const Integer&) const;
  Integer& operator/=(const Integer&);

  [[nodiscard]] Integer operator%(const Integer&) const;
  Integer& operator%=(const Integer&);

  [[nodiscard]] explicit operator bool() const;
  [[nodiscard]] explicit operator char unsigned() const;
  [[nodiscard]] explicit operator char() const;
  [[nodiscard]] explicit operator short unsigned int() const;
  [[nodiscard]] explicit operator short int() const;
  [[nodiscard]] explicit operator unsigned int() const;
  [[nodiscard]] explicit operator int() const;
  [[nodiscard]] explicit operator long unsigned int() const;
  [[nodiscard]] explicit operator long int() const;
  [[nodiscard]] explicit operator long long unsigned int() const;
  [[nodiscard]] explicit operator long long int() const;

  [[nodiscard]] bool operator==(const Integer&) const;
  [[nodiscard]] bool operator!=(const Integer&) const;
  [[nodiscard]] bool operator<(const Integer&) const;
  [[nodiscard]] bool operator<=(const Integer&) const;
  [[nodiscard]] bool operator>(const Integer&) const;
  [[nodiscard]] bool operator>=(const Integer&) const;

  friend std::ostream& operator<<(std::ostream&, const Integer&);

 private:
  std::vector<unsigned int> number_;
  bool sign_ = true;
  unsigned int base_ = 10;
};
// Integer ""_li(const char*);
}  // namespace arbitary_precision_arithmetic
