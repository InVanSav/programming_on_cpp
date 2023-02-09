#include<vector>
#include<utility>

namespace details {
struct a_help { };

template <class T, class A>
std::vector<T> make_vector(A, T value, size_t size) {
  return std::vector<T>(size, value);
}

template <class T, class A, class... Args,
    class R_T = decltype(
    make_vector<T>(A{},
                   std::declval<T>(),
                   std::declval<Args>()...)),
    class R = std::vector<R_T>
>
R make_vector(A, T value, size_t first, Args... size) {
  auto inner = make_vector<T>(A{}, value, std::forward<Args>(size)...);
  return R(first, inner);
}
}  // namespace details



template <class T, class... Args,
    class R = decltype(
    details::make_vector<T>(details::a_help{},
                            std::declval<T>(),
                            std::declval<Args>()...))
>
R initialize_vector(T value, Args... args) {
  return details::make_vector<T>(details::a_help{},
                                 value,
                                 std::forward<Args>(args)...);
}
