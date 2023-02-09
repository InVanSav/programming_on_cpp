#pragma once

#include <iterator>

template<typename T, typename Iterator>
Iterator FindIt(const T& value, Iterator first, Iterator last,
                std::random_access_iterator_tag) {
  Iterator l = first, r = last - 1, m = (r - l) / 2 + l;
  while (l < r) {
    if (*m < value) l = m + 1;
    else
      r = m;
    m = (r - l) / 2 + l;
  }
  return (*m == value ? m : last);
}

template<typename T, typename Iterator>
Iterator FindIt(const T& value, Iterator first, Iterator last,
                std::bidirectional_iterator_tag) {
  while (first != last && !(*first == value)) first++;
  return first;
}

template<typename T, typename Iterator>
Iterator Find(const T& value, Iterator first, Iterator last) {
  return FindIt(value, first, last,
                typename std::iterator_traits<Iterator>::iterator_category());
}
