#include <Integer.hpp>
#include <algorithm>
#include <utility>

namespace arbitary_precision_arithmetic {
  Integer::Integer() = default;

  Integer::Integer(const std::vector<unsigned int> &other) {
    this->number_ = other;
  }

  Integer::Integer(const Integer &other) {
    this->number_ = other.number_;
    this->sign_ = other.sign_;
  }

  Integer::Integer(Integer &&other) {
    this->number_ = other.number_;
    this->sign_ = other.sign_;
    while (!other.number_.empty()) {
      other.number_.pop_back();
    }
    other.number_.push_back(0);
    if (!other.sign_) {
      other.sign_ = true;
    }
  }

  Integer::Integer(unsigned char value) {
    while (value) {
      this->number_.push_back(value % this->base_);
      value /= this->base_;
    }
  }

  Integer::Integer(char value) {
    if (value < 0) {
      this->sign_ = !this->sign_;
    }
    auto tmp = static_cast<unsigned char>(value);
    while (tmp) {
      this->number_.push_back(tmp % this->base_);
      tmp /= this->base_;
    }
  }

  Integer::Integer(unsigned short value) {
    while (value) {
      this->number_.push_back(value % this->base_);
      value /= this->base_;
    }
  }

  Integer::Integer(short value) {
    if (value < 0) {
      this->sign_ = !this->sign_;
    }
    auto tmp = static_cast<unsigned short>(value);
    while (tmp) {
      this->number_.push_back(tmp % this->base_);
      tmp /= this->base_;
    }
  }

  Integer::Integer(unsigned int value) {
    while (value) {
      this->number_.push_back(value % this->base_);
      value /= this->base_;
    }
  }

  Integer::Integer(int value) {
    if (value < 0) {
      this->sign_ = !this->sign_;
    }
    auto tmp = static_cast<unsigned int>(value);
    while (tmp) {
      this->number_.push_back(tmp % this->base_);
      tmp /= this->base_;
    }
  }

  Integer::Integer(unsigned long value) {
    while (value) {
      this->number_.push_back(value % this->base_);
      value /= this->base_;
    }
  }

  Integer::Integer(long value) {
    if (value < 0) {
      this->sign_ = !this->sign_;
    }
    auto tmp = static_cast<unsigned long>(value);
    while (tmp) {
      this->number_.push_back(tmp % this->base_);
      tmp /= this->base_;
    }
  }

  Integer::Integer(unsigned long long value) {
    while (value) {
      this->number_.push_back(value % this->base_);
      value /= this->base_;
    }
  }

  Integer::Integer(long long value) {
    if (value < 0) {
      this->sign_ = !this->sign_;
    }
    auto tmp = static_cast<unsigned long long>(value);
    while (tmp) {
      this->number_.push_back(tmp % this->base_);
      tmp /= this->base_;
    }
  }

  Integer::Integer(const std::string &value) {
    for (auto digit : value) {
      if (digit == '-') {
        this->sign_ = !this->sign_;
      }
      this->number_.push_back(digit - '0');
    }
    std::reverse(this->number_.begin(), this->number_.end());
  }

  Integer &Integer::operator=(const Integer &other) {
    this->number_ = other.number_;
    this->sign_ = other.sign_;
    return *this;
  }

  Integer &Integer::operator=(Integer &&other) noexcept {
    if (this != &other) {
      this->number_ = other.number_;
      this->sign_ = other.sign_;
      while (!other.number_.empty()) {
        other.number_.pop_back();
      }
      other.number_.push_back(0);
      if (!other.sign_) {
        other.sign_ = true;
      }
    }
    return *this;
  }

  Integer Integer::operator-() const {
    Integer value;
    value.sign_ = false;
    return value;
  }

  Integer &Integer::operator--() {
    size_t count = 0;
    if (this->sign_) {
      if (this->number_.empty()) {
        this->sign_ = false;
        this->number_.push_back(1);
        return *this;
      }
      for (size_t i = 0; i < this->number_.size(); ++i) {
        if (this->number_[i] > 0) {
          this->number_[i]--;
          break;
        }
        count++;
      }
      for (size_t i = 0; i < count; ++i) {
        this->number_[i] = this->base_ - 1;
      }
    } else {
      count = (this->number_[0] + 1) / this->base_;
      this->number_[0] = (this->number_[0] + 1) % this->base_;
      size_t i = 1;
      while (count) {
        if (i == this->number_.size()) {
          break;
        }
        count = (this->number_[i] + 1) / this->base_;
        this->number_[i] = (this->number_[i] + 1) % this->base_;
        i++;
      }
      if ((i == this->number_.size()) && count) {
        this->number_.push_back(1);
      }
    }
    return *this;
  }

  Integer Integer::operator--(int) {
    Integer help(*this);
    size_t count = 0;
    if (this->sign_) {
      if (this->number_.empty()) {
        this->sign_ = false;
        this->number_.push_back(1);
        return help;
      }
      for (size_t i = 0; i < this->number_.size(); ++i) {
        if (this->number_[i] > 0) {
          this->number_[i]--;
          break;
        }
        count++;
      }
      for (size_t i = 0; i < count; ++i) {
        this->number_[i] = this->base_ - 1;
      }
    } else {
      count = (this->number_[0] + 1) / this->base_;
      this->number_[0] = (this->number_[0] + 1) % this->base_;
      size_t i = 1;
      while (count) {
        if (i == this->number_.size()) {
          break;
        }
        count = (this->number_[i] + 1) / this->base_;
        this->number_[i] = (this->number_[i] + 1) % this->base_;
        i++;
      }
      if ((i == this->number_.size()) && count) {
        this->number_.push_back(1);
      }
    }
    return help;
  }

  Integer Integer::operator+() const {
    return *this;
  }

  Integer& Integer::operator++() {
    size_t count = 0;
    if (!this->sign_) {
      if (this->number_.empty()) {
        this->sign_ = true;
        this->number_.push_back(1);
        return *this;
      }
      for (size_t i = 0; i < this->number_.size(); ++i) {
        if (this->number_[i] > 0) {
          this->number_[i]--;
          break;
        }
        count++;
      }
      for (size_t i = 0; i < count; ++i) {
        this->number_[i] = this->base_ - 1;
      }
    } else {
      count = (this->number_[0] + 1) / this->base_;
      this->number_[0] = (this->number_[0] + 1) % this->base_;
      size_t i = 1;
      while (count) {
        if (i == this->number_.size()) {
          break;
        }
        count = (this->number_[i] + 1) / this->base_;
        this->number_[i] = (this->number_[i] + 1) % this->base_;
        i++;
      }
      if ((i == this->number_.size()) && count) {
        this->number_.push_back(1);
      }
    }
    return *this;
  }

  Integer Integer::operator++(int) {
    Integer help(*this);
    size_t count = 0;
    if (!this->sign_) {
      if (this->number_.empty()) {
        this->sign_ = true;
        this->number_.push_back(1);
        return help;
      }
      for (size_t i = 0; i < this->number_.size(); ++i) {
        if (this->number_[i] > 0) {
          this->number_[i]--;
          break;
        }
        count++;
      }
      for (size_t i = 0; i < count; ++i) {
        this->number_[i] = this->base_ - 1;
      }
    } else {
      count = (this->number_[0] + 1) / this->base_;
      this->number_[0] = (this->number_[0] + 1) % this->base_;
      size_t i = 1;
      while (count) {
        if (i == this->number_.size()) {
          break;
        }
        count = (this->number_[i] + 1) / this->base_;
        this->number_[i] = (this->number_[i] + 1) % this->base_;
        i++;
      }
      if ((i == this->number_.size()) && count) {
        this->number_.push_back(1);
      }
    }
    return help;
  }

  Integer Integer::operator-(const Integer &other) const {
    std::vector<unsigned int> result;
    size_t carry = 0;
    for (size_t i = 0; i < other.number_.size(); i++) {
      size_t cur_diff = this->number_[i] - other.number_[i] - carry;
      if (cur_diff < 0) {
        cur_diff += 10;
        carry = 1;
      } else {
        carry = 0;
      }
      result.push_back(cur_diff);
    }
    for (size_t i = this->number_.size(); i < other.number_.size(); i++) {
      if (other.number_[i] - carry < 0) {
        result.push_back(other.number_[i] + 10 - carry);
        carry = 1;
      } else {
        result.push_back(other.number_[i] - carry);
        carry = 0;
      }
    }
    while (result.size() > 1 && !result.back()) {
      result.pop_back();
    }
    return {result};
  }

  Integer &Integer::operator-=(const Integer &other) {
    std::vector<unsigned int> result;
    size_t carry = 0;
    for (size_t i = 0; i < this->number_.size(); i++) {
      size_t cur_diff = other.number_[i] - this->number_[i] - carry;
      if (cur_diff < 0) {
        cur_diff += 10;
        carry = 1;
      } else {
        carry = 0;
      }
      result.push_back(cur_diff);
    }
    for (size_t i = this->number_.size(); i < other.number_.size(); i++) {
      if (other.number_[i] - carry < 0) {
        result.push_back(other.number_[i] + 10 - carry);
        carry = 1;
      } else {
        result.push_back(other.number_[i] - carry);
        carry = 0;
      }
    }
    while (result.size() > 1 && !result.back()) {
      result.pop_back();
    }
    Integer help(result);
    *this = std::move(help);
    return *this;
  }

  Integer Integer::operator+(const Integer &other) const {
    std::vector<unsigned int> result;
    size_t carry = 0;
    for (size_t i = 0;
         i < std::min(other.number_.size(), this->number_.size()); i++) {
      size_t cur_sum = carry + other.number_[i] + this->number_[i];
      result.push_back(cur_sum % this->base_);
      carry = cur_sum / this->base_;
    }
    for (size_t i = std::min(other.number_.size(), this->number_.size());
         i < std::max(other.number_.size(), this->number_.size()); i++) {
      if (other.number_.size() > this->number_.size()) {
          size_t cur_sum = carry + other.number_[i];
          result.push_back(cur_sum % this->base_);
          carry = cur_sum / this->base_;
      } else {
          size_t cur_sum = carry + this->number_[i];
          result.push_back(cur_sum % this->base_);
          carry = cur_sum / this->base_;
      }
    }
    if (carry) {
      result.push_back(carry);
    }
    return {result};
  }

  Integer &Integer::operator+=(const Integer &other) {
    std::vector<unsigned int> result;
    size_t carry = 0;
    for (size_t i = 0;
         i < std::min(other.number_.size(), this->number_.size()); i++) {
      size_t cur_sum = carry + other.number_[i] + this->number_[i];
      result.push_back(cur_sum % this->base_);
      carry = cur_sum / this->base_;
    }
    for (size_t i = std::min(other.number_.size(), this->number_.size());
         i < std::max(other.number_.size(), this->number_.size()); i++) {
      if (other.number_.size() > this->number_.size()) {
        size_t cur_sum = carry + other.number_[i];
        result.push_back(cur_sum % this->base_);
        carry = cur_sum / this->base_;
      } else {
        size_t cur_sum = carry + this->number_[i];
        result.push_back(cur_sum % this->base_);
        carry = cur_sum / this->base_;
      }
    }
    if (carry) {
      result.push_back(carry);
    }
    Integer help(result);
    *this = std::move(help);
    return *this;
  }

  Integer Integer::operator*(const Integer &other) const {
    std::vector<unsigned int> result(
        other.number_.size() * this->number_.size() + 1, 0);
    for (size_t i = 0; i < other.number_.size(); i++) {
      size_t carry = 0;
      for (size_t j = 0; j < this->number_.size(); j++) {
        auto cur = result[i+j] + other.number_[i] * this->number_[j] + carry;
        carry = cur / other.base_;
        result[i+j] = cur % this->base_;
      }
      auto cur = this->number_.size();
      while (carry) {
        result[i+cur] = carry % this->base_;
        cur++;
        carry /= this->base_;
      }
    }
    while (result.size() > 1 && !result.back()) {
      result.pop_back();
    }
    return {result};
  }

  Integer &Integer::operator*=(const Integer &other) {
    std::vector<unsigned int> result(
        other.number_.size() * this->number_.size() + 1, 0);
    for (size_t i = 0; i < other.number_.size(); i++) {
      size_t carry = 0;
      for (size_t j = 0; j < this->number_.size(); j++) {
        auto cur = result[i+j] + other.number_[i] * this->number_[j] + carry;
        carry = cur / other.base_;
        result[i+j] = cur % this->base_;
      }
      auto cur = this->number_.size();
      while (carry) {
        result[i+cur] = carry % this->base_;
        cur++;
        carry /= this->base_;
      }
    }
    while (result.size() > 1 && !result.back()) {
      result.pop_back();
    }
    Integer help(result);
    *this = std::move(help);
    return *this;
  }

  Integer Integer::operator/(const Integer &other) const {
    std::vector<unsigned int> result(
        this->number_.size() - other.number_.size() + 1, 0);
    Integer help(result);
    for (size_t i = result.size() - 1; i >= 0; i--) {
      while (other * help <= *this) {
        help.number_[i]++;
      }
      help.number_[i]--;
    }
    while (help.number_.size() > 1 && !help.number_.back()) {
      help.number_.pop_back();
    }
    return help;
  }

  Integer &Integer::operator/=(const Integer &other) {
    std::vector<unsigned int> result(
        other.number_.size() * this->number_.size() + 1, 0);
    Integer help(result);
    for (size_t i = result.size() - 1; i >= 0; i--) {
      while (other * help <= *this) {
        help.number_[i]++;
      }
      help.number_[i]--;
    }
    while (help.number_.size() > 1 && !help.number_.back()) {
      help.number_.pop_back();
    }
    *this = std::move(help);
    return *this;
  }

  Integer Integer::operator%(const Integer &other) const {
    Integer help = other - (other / *this) * (*this);
    return help;
  }

  Integer &Integer::operator%=(const Integer &other) {
    Integer help = other - (other / *this) * (*this);
    *this = std::move(help);
    return *this;
  }

  bool Integer::operator==(const Integer &other) const {
    if (this->number_.size() == other.number_.size()) {
      if (this->sign_ == other.sign_) {
        for (size_t i = 0; i < this->number_.size(); ++i) {
          if (this->number_[i] != other.number_[i]) {
            return false;
          }
        }
        return true;
      }
    }
    return false;
  }

  bool Integer::operator!=(const Integer &other) const {
    if (this->number_.size() == other.number_.size()) {
      if (this->sign_ == other.sign_) {
        for (size_t i = 0; i < this->number_.size(); ++i) {
          if (this->number_[i] != other.number_[i]) {
            return true;
          }
        }
        return false;
      }
    }
    return true;
  }

  bool Integer::operator<(const Integer &other) const {
    if (other.number_.size() < this->number_.size()) {
      return true;
    } else if (other.number_.size() > this->number_.size()) {
      return false;
    } else {
      for (size_t i = other.number_.size() - 1; i > 0; i--) {
        if (other.number_[i] > this->number_[i]) {
          return false;
        }
        if (other.number_[i] < this->number_[i]) {
          return true;
        }
      }
      return true;
    }
  }

  bool Integer::operator<=(const Integer &other) const {
    if (other.number_.size() <= this->number_.size()) {
      return true;
    } else if (other.number_.size() > this->number_.size()) {
      return false;
    } else {
      for (size_t i = other.number_.size() - 1; i > 0; i--) {
        if (other.number_[i] > this->number_[i]) {
          return false;
        }
        if (other.number_[i] <= this->number_[i]) {
          return true;
        }
      }
      return true;
    }
  }

  bool Integer::operator>(const Integer &other) const {
    if (other.number_.size() < this->number_.size()) {
      return false;
    } else if (other.number_.size() > this->number_.size()) {
      return true;
    } else {
      for (size_t i = other.number_.size() - 1; i > 0; i--) {
        if (other.number_[i] > this->number_[i]) {
          return true;
        }
        if (other.number_[i] < this->number_[i]) {
          return false;
        }
      }
      return false;
    }
  }

  bool Integer::operator>=(const Integer &other) const {
    if (other.number_.size() >= this->number_.size()) {
      return true;
    } else if (other.number_.size() < this->number_.size()) {
      return false;
    } else {
      for (size_t i = other.number_.size() - 1; i > 0; i--) {
        if (other.number_[i] < this->number_[i]) {
          return false;
        }
        if (other.number_[i] >= this->number_[i]) {
          return true;
        }
      }
      return true;
    }
  }

  Integer::~Integer() {
    this->number_.clear();
  }

  std::ostream& operator<<(std::ostream &os, const Integer &number) {
    if (!number.sign_) {
      os << "-";
    }
    for (auto i = number.number_.size(); i > 0; i--) {
      os << number.number_[i-1];
    }
    return os;
  }

}  // namespace arbitary_precision_arithmetic
