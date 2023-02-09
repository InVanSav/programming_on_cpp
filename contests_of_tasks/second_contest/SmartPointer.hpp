#pragma once

#include <memory>
#include "Test.hpp"

namespace smart_pointer {
class exception : std::exception {
  using base_class = std::exception;
  using base_class::base_class;
};

template<typename T, typename Allocator>
class SmartPointer {
  ENABLE_CLASS_TESTS;

 public:
  using value_type = T;

  explicit SmartPointer(value_type *ptr = nullptr) {
    if (ptr != nullptr) {
      core = new Core(ptr);
    } else {
      core = nullptr;
    }
  }

  SmartPointer(const SmartPointer &other) {
    core = other.core;
    if (core != nullptr) {
      core->count_++;
    }
  }

  SmartPointer(SmartPointer &&other)  noexcept {
    core = other.core;
    other.core = nullptr;
  }

  SmartPointer &operator=(const SmartPointer &other) {
    if (this != &other) {
      this->~SmartPointer();
      core = other.core;
      if (core != nullptr) {
        core->count_++;
      }
    }
    return *this;
  }

  SmartPointer &operator=(SmartPointer &&other) {
    if (this != &other) {
      this->~SmartPointer();
      core = other.core;
      other.core = nullptr;
    }
    return *this;
  }

  SmartPointer &operator=(value_type *ptr) {
    this->~SmartPointer();
    if (ptr != nullptr) {
      core = new Core(ptr);
    } else {
      core = nullptr;
    }
    return *this;
  }

  ~SmartPointer() {
    if (core != nullptr) {
      core->count_--;
      if (core->count_ == 0) {
        delete core;
      }
    }
  }

  value_type &operator*() {
    if (core != nullptr) {
      return *(core->ptr_);
    } else {
      throw smart_pointer::exception();
    }
  }
  const value_type &operator*() const {
    if (core != nullptr) {
      return *(core->ptr_);
    } else {
      throw smart_pointer::exception();
    }
  }

  value_type *operator->() const {
    if (core != nullptr) {
      return core->ptr_;
    } else {
      return nullptr;
    }
  }

  value_type *get() const {
    if (core != nullptr) {
      return core->ptr_;
    } else {
      return nullptr;
    }
  }

  explicit operator bool() const {
    if (core == nullptr) {
      return false;
    }
    return true;
  }

  template<typename U, typename AnotherAllocator>
  friend
  class SmartPointer;

  template<typename U, typename AnotherAllocator>
  bool operator==(const SmartPointer<U, AnotherAllocator> &other) const {
    if (static_cast<void *>(core) == static_cast<void *>(other.core)) {
      return true;
    }
    return false;
  }

  template<typename U, typename AnotherAllocator>
  bool operator!=(const SmartPointer<U, AnotherAllocator> &other) const {
    if (static_cast<void *>(core) == static_cast<void *>(other.core)) {
      return false;
    }
    return true;
  }

  [[nodiscard]] std::size_t count_owners() const {
    if (core != nullptr) {
      return core->count_;
    } else {
      return 0;
    }
  }

 private:
  class Core {
   public:
    explicit Core(T *ptr = 0) {
      ptr_ = ptr;
      if (ptr != 0) {
        count_ = 1;
      } else {
        count_ = 0;
      }
    }
    ~Core() {
      delete ptr_;
    }
    size_t count_;
    T *ptr_;
  };
  Core *core;
};
}  // namespace smart_pointer
