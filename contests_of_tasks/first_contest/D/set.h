#include <iostream>
#include <utility>
#include <vector>
#include <algorithm>
#include <cstdint>

class Set {
  std::vector<int64_t> vec;

 public:
  Set() = default;
  explicit Set(std::vector<int64_t> m) {
    Set::vec = std::vector<int64_t>(std::move(m));

    std::sort(Set::vec.begin(), Set::vec.end());
    std::vector<int64_t, std::allocator<int64_t>>::iterator last;
    last = std::unique(Set::vec.begin(), Set::vec.end());
    Set::vec.erase(last, Set::vec.end());
  }

  void Add(int64_t x) {
    if (!Set::Contains(x)) Set::vec.push_back(x);
  }

  void Remove(int64_t x) {
    Set::vec.erase(std::remove(Set::vec.begin(), Set::vec.end(), x),
                   Set::vec.end());
  }

  bool Contains(int64_t x) {
    return std::find(Set::vec.begin(), Set::vec.end(), x) != Set::vec.end();
  }

  Set Union(const Set &other) const {
    std::vector<int64_t> v(Set::vec);
    for (int64_t i : other.vec) {
      v.push_back(i);
    }
    return Set(v);
  }

  Set Intersection(const Set &other) const {
    std::vector<int64_t> v;
    for (int64_t i : other.vec) {
      if (std::find(Set::vec.begin(), Set::vec.end(), i) != Set::vec.end()) {
        v.push_back(i);
      }
    }
    return Set(v);
  }

  Set Difference(const Set &other) const {
    std::vector<int64_t> v;
    for (int64_t i : Set::vec) {
      if (std::find(other.vec.begin(), other.vec.end(), i) == other.vec.end()) {
        v.push_back(i);
      }
    }
    return Set(v);
  }

  Set SymmetricDifference(const Set &other) const {
    std::vector<int64_t> v;
    for (int64_t i : Set::vec) {
      if (std::find(other.vec.begin(), other.vec.end(), i) == other.vec.end()) {
        v.push_back(i);
      }
    }
    for (int64_t i : other.vec) {
      if (std::find(Set::vec.begin(), Set::vec.end(), i) == Set::vec.end()) {
        v.push_back(i);
      }
    }
    return Set(v);
  }

  std::vector<int64_t> Data() const {
    std::vector<int64_t> vect(Set::vec);
    return vect;
  }
};
