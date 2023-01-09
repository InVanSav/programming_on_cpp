template <typename T, typename V>
class is_customly_convertible {
  struct char2 { char c[2]; };

  template <typename C> static char test(decltype(&C::operator()));
  template <typename C> static char2 test(...);

 public:
  enum { value = sizeof(test<Convert<T, V>>(0)) == sizeof(char) };
};

class NoCopyConstructible {
 public:
  NoCopyConstructible(const NoCopyConstructible&) = delete;
  NoCopyConstructible& operator=(const NoCopyConstructible&) = delete;
};

class NoTriviallyConstructible {
 public:
  NoTriviallyConstructible() = delete;
};

template<>
struct Convert<NoCopyConstructible, NoTriviallyConstructible> {
  int operator()(const int &a) {
    return a;
  }
};

template<>
struct Convert<NoTriviallyConstructible, int> {
  int operator()(const int &a) {
    return a;
  }
};
