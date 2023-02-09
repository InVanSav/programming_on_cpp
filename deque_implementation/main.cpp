#include <iostream>
#include <cassert>
#include "Deque.hpp"

namespace fefu_laboratory_two {

void allocator() {

  CustomAllocationStrategy<> allocation_area{};

  {
    CustomAllocator<int> custom_int_allocator{ allocation_area };
    CustomAllocator<float> custom_float_allocator { custom_int_allocator };
    custom_list<float> list{ { 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f },
                             custom_float_allocator };
    for (float val : list) {
      std::cout << val << " ";
    }

    std::cout << std::endl << std::endl;
  }

  {
    CustomAllocator<double> custom_double_allocator{ allocation_area };
    custom_set<double> set{ { 1000.0, 2000.0, 3000.0 },
                            custom_double_allocator };
    for (double val : set) {
      std::cout << val << " ";
    }

    std::cout << std::endl << std::endl;
  }

  {
    CustomAllocator<char> custom_char_allocator{ allocation_area };
    custom_string string_1{"First allocated string without SBO ",
                           custom_char_allocator };
    custom_string string_2{ "Second allocated string without SBO ",
                           custom_char_allocator };
    custom_string string_3{ "Third allocated string without SBO ",
                           custom_char_allocator };
    custom_string result_string = string_1 + string_2 + string_3;

    std::cout << result_string;
    std::cout << std::endl << std::endl;
  }

}

void iterators() {

  {
    std::vector<int> arr = { 1, 2, 3, 4, 5 };
    int* p = &arr[1];
    Deque_iterator<int> it = Deque_iterator<int>(p);
    assert(*it == 2);
  }

  {
    std::vector<int> arr = { 1, 2, 3, 4, 5 };
    int* p1 = &arr[1];
    int* p2 = &arr[3];
    Deque_iterator<int> it1(p1);
    Deque_iterator<int> it2(p2);
    swap(it1, it2);
    assert(*it1 == 4);
  }

  {
    std::vector<int> arr = { 1, 2, 3, 4, 5 };
    int* p1 = &arr[1];
    int* p2 = &arr[3];
    Deque_iterator<int> it1(p1);
    Deque_iterator<int> it2(p2);
    assert(!(it1 == it2));

    it2 = it1;
    assert(it1 == it2);
  }

  {
    std::vector<int> arr = { 1, 2, 3, 4, 5 };
    int* p1 = &arr[1];
    int* p2 = &arr[3];
    Deque_iterator<int> it1(p1);
    Deque_iterator<int> it2(p2);
    assert(it1 != it2);

    it2 = it1;
    assert(!(it1 != it2));
  }

  {
    std::vector<int> arr = { 1, 2, 3, 4, 5 };
    int* p1 = &arr[1];
    Deque_iterator<int> it(p1);
    assert(*it == 2);
  }

  {
    std::vector<int> arr = { 1, 2, 3, 4, 5 };
    int* p1 = &arr[1];
    Deque_iterator<int> it(p1);
    auto pi = &it;
    auto p2 = pi->current_;
    assert(p2 == p1);
  }

  {
    std::vector<int> arr = { 1, 2, 3, 4, 5 };
    int* p1 = &arr[1];
    Deque_iterator<int> it(p1);
    ++it;
    assert(*it == 3);
  }

  {
    std::vector<int> arr = { 1, 2, 3, 4, 5 };
    int* p1 = &arr[1];
    Deque_iterator<int> it(p1);
    Deque_iterator<int> it2 = it++;
    assert(*it == 3);
    assert(*it2 == 2);
  }

  {
    std::vector<int> arr = { 1, 2, 3, 4, 5 };
    int* p1 = &arr[1];
    Deque_iterator<int> it(p1);
    --it;
    assert(*it == 1);
  }

  {
    std::vector<int> arr = { 1, 2, 3, 4, 5 };
    int* p1 = &arr[1];
    Deque_iterator<int> it(p1);
    Deque_iterator<int> it2 = it--;
    assert(*it == 1);
    assert(*it2 == 2);
  }

  {
    std::vector<int> arr = { 1, 2, 3, 4, 5 };
    int* p1 = &arr[1];
    Deque_iterator<int> it(p1);
    it += 2;
    Deque_iterator<int> it2(it);
    assert(*it2 == 4);
  }

  {
    std::vector<int> arr = { 1, 2, 3, 4, 5 };
    int* p1 = &arr[1];
    Deque_iterator<int> it(p1);
    it += 2;
    assert(*it == 4);
  }

  {
    std::vector<int> arr = { 1, 2, 3, 4, 5 };
    int* p1 = &arr[3];
    Deque_iterator<int> it(p1);
    it -= 2;
    Deque_iterator<int> it2(it);
    assert(*it2 == 2);
  }

  {
    std::vector<int> arr = { 1, 2, 3, 4, 5 };
    int* p1 = &arr[3];
    Deque_iterator<int> it(p1);
    it -= 2;
    assert(*it == 2);
  }

  {
    std::vector<int> arr = { 1, 2, 3, 4, 5 };
    int* p1 = &arr[1];
    Deque_iterator<int> it(p1);
    assert(it[1] == 3);
  }

  //Deque_const_iterator

  {
    std::vector<int> arr = { 1, 2, 3, 4, 5 };
    int* p = &arr[1];
    Deque_const_iterator<int> it = Deque_const_iterator<int>(p);
    assert(*it == 2);
  }

  {
    std::vector<int> arr = { 1, 2, 3, 4, 5 };
    int* p1 = &arr[1];
    int* p2 = &arr[3];
    Deque_const_iterator<int> it1(p1);
    Deque_const_iterator<int> it2(p2);
    swap(it1, it2);
    assert(*it1 == 4);
  }

  {
    std::vector<int> arr = { 1, 2, 3, 4, 5 };
    int* p1 = &arr[1];
    int* p2 = &arr[3];
    Deque_const_iterator<int> it1(p1);
    Deque_const_iterator<int> it2(p2);
    assert(!(it1 == it2));

    it2 = it1;
    assert(it1 == it2);
  }

  {
    std::vector<int> arr = { 1, 2, 3, 4, 5 };
    int* p1 = &arr[1];
    int* p2 = &arr[3];
    Deque_const_iterator<int> it1(p1);
    Deque_const_iterator<int> it2(p2);
    assert(it1 != it2);

    it2 = it1;
    assert(!(it1 != it2));
  }

  {
    std::vector<int> arr = { 1, 2, 3, 4, 5 };
    int* p1 = &arr[1];
    Deque_const_iterator<int> it(p1);
    assert(*it == 2);
  }

  {
    std::vector<int> arr = { 1, 2, 3, 4, 5 };
    int* p1 = &arr[1];
    Deque_const_iterator<int> it(p1);
    auto pi = &it;
    auto p2 = pi->current_;
    assert(p2 == p1);
  }

  {
    std::vector<int> arr = { 1, 2, 3, 4, 5 };
    int* p1 = &arr[1];
    Deque_const_iterator<int> it(p1);
    Deque_const_iterator<int> it2 = it++;
    assert(*it == 3);
    assert(*it2 == 2);
  }

  {
    std::vector<int> arr = { 1, 2, 3, 4, 5 };
    int* p1 = &arr[1];
    Deque_const_iterator<int> it(p1);
    ++it;
    assert(*it == 3);
  }

  {
    std::vector<int> arr = { 1, 2, 3, 4, 5 };
    int* p1 = &arr[1];
    Deque_const_iterator<int> it(p1);
    --it;
    assert(*it == 1);
  }

  {
    std::vector<int> arr = { 1, 2, 3, 4, 5 };
    int* p1 = &arr[1];
    Deque_const_iterator<int> it(p1);
    Deque_const_iterator<int> it2 = it--;
    assert(*it == 1);
    assert(*it2 == 2);
  }

  {
    std::vector<int> arr = { 1, 2, 3, 4, 5 };
    int* p1 = &arr[1];
    Deque_const_iterator<int> it(p1);
    it += 2;
    Deque_const_iterator<int> it2(it);
    assert(*it2 == 4);
  }

  {
    std::vector<int> arr = { 1, 2, 3, 4, 5 };
    int* p1 = &arr[1];
    Deque_const_iterator<int> it(p1);
    it += 2;
    assert(*it == 4);
  }

  {
    std::vector<int> arr = { 1, 2, 3, 4, 5 };
    int* p1 = &arr[3];
    Deque_const_iterator<int> it(p1);
    it -= 2;
    Deque_const_iterator<int> it2(it);
    assert(*it2 == 2);
  }

  {
    std::vector<int> arr = { 1, 2, 3, 4, 5 };
    int* p1 = &arr[3];
    Deque_const_iterator<int> it(p1);
    it -= 2;
    assert(*it == 2);
  }

  {
    std::vector<int> arr = { 1, 2, 3, 4, 5 };
    int* p1 = &arr[1];
    Deque_const_iterator<int> it(p1);
    assert(it[1] == 3);
  }

  //Deque_reverse_iterator

  using rIt = Deque_reverse_iterator<Deque_iterator<int>>;

  {
    std::vector<int> arr = {1, 2, 3, 4, 5};
    int *p = &arr[1];
    Deque_iterator<int> it(p);
    rIt rit(it);
    assert(*rit==2);
  }

  {
    std::vector<int> arr = {1, 2, 3, 4, 5};
    int *p = &arr[1];
    Deque_iterator<int> it(p);
    rIt rit(it);
    rIt rit2(rit);
    assert(*rit2==2);
  }

  {
    std::vector<int> arr = {1, 2, 3, 4, 5};
    int *p = &arr[1];
    Deque_iterator<int> it(p);
    rIt rit(it);
    rIt rit2 = rit;
    assert(*rit2==2);
  }

  {
    std::vector<int> arr = {1, 2, 3, 4, 5};
    int *p = &arr[1];
    Deque_iterator<int> it(p);
    rIt rit(it);
    assert(rit.base()==it);
  }

  {
    std::vector<int> arr = {1, 2, 3, 4, 5};
    int *p = &arr[1];
    Deque_iterator<int> it(p);
    rIt rit(it);
    assert(*rit==2);
  }

  {
    std::vector<int> arr = {1, 2, 3, 4, 5};
    int *p = &arr[3];
    Deque_iterator<int> it(p);
    rIt rit(it);
    ++
    rit;
    assert(*rit==3);
  }

  {
    std::vector<int> arr = {1, 2, 3, 4, 5};
    int *p = &arr[3];
    Deque_iterator<int> it(p);
    rIt rit(it);
    rIt rit2 = rit++;
    assert(*rit2==4);
    assert(*rit==3);
  }

  {
    std::vector<int> arr = {1, 2, 3, 4, 5};
    int *p = &arr[2];
    Deque_iterator<int> it(p);
    rIt rit(it);
    --rit;
    assert(*rit==4);
  }

  {
    std::vector<int> arr = {1, 2, 3, 4, 5};
    int *p = &arr[2];
    Deque_iterator<int> it(p);
    rIt rit(it);
    rIt rit2 = rit--;
    assert(*rit2==3);
    assert(*rit==4);
  }

  {
    std::vector<int> arr = {1, 2, 3, 4, 5};
    int *p = &arr[3];
    Deque_iterator<int> it(p);
    rIt rit(it);
    rit += 2;
    assert(*rit==2);
  }

  {
    std::vector<int> arr = {1, 2, 3, 4, 5};
    int *p = &arr[2];
    Deque_iterator<int> it(p);
    rIt rit(it);
    rit -= 2;
    assert(*rit==5);
  }

  {
    std::vector<int> arr = {1, 2, 3, 4, 5};
    int *p = &arr[2];
    Deque_iterator<int> it(p);
    rIt rit(it);
    rIt rit2(it);
    assert(rit==rit2);
    rit++;
    assert(!(rit==rit2));
  }

  {
    std::vector<int> arr = {1, 2, 3, 4, 5};
    int *p = &arr[2];
    Deque_iterator<int> it(p);
    rIt rit(it);
    rIt rit2(it);
    assert(!(rit!=rit2));
    rit++;
    assert(rit!=rit2);
  }

  {
    std::vector<int> arr = {1, 2, 3, 4, 5};
    int *p = &arr[3];
    Deque_iterator<int> it(p);
    rIt rit(it);
    it -= 2;
    rIt rit2(it);
    assert(rit - rit2 == -2);
    assert(rit2 - rit == 2);
    assert(rit2 - rit2 == 0);
  }

}

template<typename T>
using custom_deque = Deque<T, CustomAllocator<T>>;

void deque() {

  CustomAllocationStrategy<> allocation_area{};

  {
    CustomAllocator<int> allocator{ allocation_area };
    custom_deque<int> deque{ allocator };

    deque.resize(16);
    for (int i = 0; i < 16; ++i) {
      std::cout << deque.at(i) << " ";
    }

    custom_deque<int> deque_1( 10, 15 );

    std::cout << std::endl << std::endl;
    deque.clear();
    std::cout << deque.size() << " ";
    std::cout << std::endl << std::endl;
  }

  {
    CustomAllocator<int> allocator{ allocation_area };
    custom_deque<int> deque_1{allocator };
    custom_deque<int> deque{ deque_1, allocator };
    deque.resize(16);
    for (int i = 0; i < 16; ++i) {
      std::cout << deque[i] << " ";
    }
    std::cout << std::endl << std::endl;
  }

  {
    CustomAllocator<int> allocator{ allocation_area };
    custom_deque<int> deque{ allocator };

    deque.resize(16);
    std::cout << deque.front() << std::endl;
    std::cout << deque.back() << std::endl;
    std::cout << deque.empty() << std::endl;

    std::cout << deque.max_size() << std::endl << std::endl;
  }

  {
    CustomAllocator<int> allocator{ allocation_area };
    custom_deque<int> deque_1{ allocator };
    custom_deque<int> deque{ allocator };
    deque.swap(deque_1);
  }

}

} //namespace fefu_laboratory_two;

int main() {
  fefu_laboratory_two::iterators();
  fefu_laboratory_two::allocator();
  fefu_laboratory_two::deque();
  return 0;
}
