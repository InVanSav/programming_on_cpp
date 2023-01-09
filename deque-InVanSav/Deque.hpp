#pragma once

#include <type_traits>
#include <set>
#include <iterator>
#include <memory>
#include <deque>
#include <vector>
#include <list>
#include <set>
#include <unordered_set>

namespace fefu_laboratory_two {
template <typename T, class AllocationStrategy>
class Allocator {
 public:
  using value_type = T;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using pointer = T*;
  using const_pointer = const T*;
  using reference = T&;
  using const_reference = const T&;
//  using allocation_result = std::allocation_result<pointer>;

  Allocator() noexcept = default;

  explicit Allocator(AllocationStrategy& strategy) noexcept
      : m_allocation_strategy(&strategy) {}

  Allocator(const Allocator& other) noexcept
      : m_allocation_strategy(other.m_allocation_strategy) {}

  template<typename U>
  explicit Allocator(const Allocator<U, AllocationStrategy>& other) noexcept
      : m_allocation_strategy(other.m_allocation_strategy) {}

  pointer allocate(size_type size) {
    return static_cast<pointer>(
        m_allocation_strategy->allocate(size * sizeof(value_type)));
  }

  void deallocate(pointer p, size_type n) noexcept {
    m_allocation_strategy->deallocate(p, n * sizeof(value_type));
  }

  template<typename U, typename... Args>
  void construct(U* ptr, Args&&... args) {
    new (reinterpret_cast<void*>(ptr)) U { std::forward<Args>(args)... };
  }

  template<typename U>
  void destroy(U* ptr) {
    ptr->~U();
  }

//  [[nodiscard]] allocation_result allocate_at_least(size_type n) {
//    return allocation_result {
//        reinterpret_cast<pointer>(::operator new(n * sizeof(value_type))), n
//    };
//  }

  AllocationStrategy* m_allocation_strategy = nullptr;

};

namespace details {

std::size_t getAlignmentPadding(
    std::size_t not_aligned_address, std::size_t alignment) {
  if ( (alignment != 0u) && (not_aligned_address % alignment != 0u) ) {
    const std::size_t multiplier = (not_aligned_address / alignment) + 1u;
    const std::size_t aligned_address = multiplier * alignment;
    return aligned_address - not_aligned_address;
  }
  return 0u;
}

template<std::size_t CHUNK_SIZE>
class Chunk {
  static constexpr std::size_t HEADER_SIZE = 4u;
  static_assert(CHUNK_SIZE % HEADER_SIZE == 0,
                "CHUNK_SIZE must be multiple of the four");
  static_assert(CHUNK_SIZE > HEADER_SIZE,
                "CHUNK_SIZE must be more than HEADER_SIZE");

 public:
  Chunk() {
    memory_blocks_.resize(CHUNK_SIZE);
    auto* init_header =
        reinterpret_cast<std::uint32_t*>(memory_blocks_.data());
    *init_header = CHUNK_SIZE - HEADER_SIZE;
    memory_max_block_ = init_header;
    memory_free_blocks_.insert(init_header);
  }

  bool isInside(const std::uint8_t* address) const noexcept {
    const auto* start_chunk_address =
        reinterpret_cast<const std::uint8_t*>(memory_blocks_.data());
    const std::uint8_t* end_chunk_address = start_chunk_address + CHUNK_SIZE;
    return (start_chunk_address <= address) && (address <= end_chunk_address);
  }

  std::uint8_t* tryReserveBlock(std::size_t allocation_size) {
    const std::size_t not_aligned_address =
        reinterpret_cast<std::size_t>(memory_max_block_) + allocation_size;
    const std::size_t alignment_padding =
        getAlignmentPadding(not_aligned_address, HEADER_SIZE);
    const auto allocation_size_with_alignment =
        static_cast<std::uint32_t>(allocation_size + alignment_padding);

    if (
        (!memory_max_block_) ||
            (allocation_size_with_alignment > *memory_max_block_) ) {
      return nullptr;
    }

    const auto min_it =
        std::min_element(memory_free_blocks_.cbegin(),
                         memory_free_blocks_.cend(),
                         [allocation_size_with_alignment]
                         (const std::uint32_t* lhs, const std::uint32_t* rhs) {
                           if (*rhs < allocation_size_with_alignment) {
                             return true;
                           }
                           return (*lhs < *rhs) && (*lhs >= allocation_size_with_alignment);
                         });

    assert(min_it != memory_free_blocks_.cend() &&
        "Internal logic error with reserve block, "
        "something wrong in implementation...");
    assert(**min_it >= allocation_size_with_alignment &&
        "Internal logic error with reserve block, "
        "something wrong in implementation...");

    std::uint32_t* header_address = *min_it;
    auto* new_header_address =
        reinterpret_cast<std::uint32_t*>(
            reinterpret_cast<std::uint8_t*>(header_address) +
                HEADER_SIZE +
                allocation_size_with_alignment);

    if (memory_free_blocks_.find(new_header_address)
        == memory_free_blocks_.cend()) {
      const std::uint32_t old_block_size = *header_address;
      const std::uint32_t difference = old_block_size - HEADER_SIZE;
      if (difference >= allocation_size_with_alignment) {
        const std::uint32_t new_block_size =
            difference - allocation_size_with_alignment;
        *new_header_address = new_block_size;
        memory_free_blocks_.insert(new_header_address);
      }
    }

    memory_free_blocks_.erase(header_address);
    *header_address = static_cast<std::uint32_t>(allocation_size);
    if (header_address == memory_max_block_) {
      const auto max_it =
          std::max_element(
              memory_free_blocks_.cbegin(),
              memory_free_blocks_.cend(),
              [] (const std::uint32_t* lhs, const std::uint32_t* rhs) {
                return (*lhs) < (*rhs);
              });

      memory_max_block_ =
          (max_it != memory_free_blocks_.cend()) ? (*max_it) : (nullptr);
    }

    return reinterpret_cast<std::uint8_t*>(header_address) + HEADER_SIZE;
  }

  void releaseBlock(std::uint8_t* block_ptr) {
    std::uint8_t* header_address = block_ptr - HEADER_SIZE;
    const std::uint32_t size_relized_block = *header_address;
    if ( (!memory_max_block_) || (size_relized_block > *memory_max_block_) ) {
      memory_max_block_ = reinterpret_cast<std::uint32_t*>(header_address);
    }

    memory_free_blocks_.insert(
        reinterpret_cast<std::uint32_t*>(header_address));
    auto forward_it =
        memory_free_blocks_.find(
            reinterpret_cast<std::uint32_t*>(header_address));
    auto backward_it =
        tryDefragment(forward_it, memory_free_blocks_.end());
    tryDefragment(
        std::make_reverse_iterator(backward_it),
        memory_free_blocks_.rend());
  }

 private:
  template<typename DstIterator, typename SrcIterator>
  constexpr DstIterator getIterator(SrcIterator it) const {

    using iterator = std::set<std::uint32_t*>::iterator;
    using reverse_iterator = std::set<std::uint32_t*>::reverse_iterator;

    if constexpr (
        std::is_same<SrcIterator, iterator>::value &&
            std::is_same<DstIterator, reverse_iterator>::value ) {
      return std::make_reverse_iterator(it);
    } else if constexpr (
        (std::is_same<SrcIterator, reverse_iterator>::value ) &&
            (std::is_same<DstIterator, iterator>::value) ) {
      return it.base();
    } else {
      return it;
    }
  }

  template<typename Iterator>
  Iterator tryDefragment(Iterator start_it, Iterator end_it) {

    auto current_it = start_it++;
    auto next_it = start_it;
    std::uint32_t* current_header_address = *current_it;

    if ( (current_it != end_it) && (next_it != end_it) ) {
      std::uint32_t* next_header_address = *next_it;
      const std::uint32_t current_block_size = *current_header_address;
      const std::uint32_t* available_current_block_address =
          reinterpret_cast<std::uint32_t*>(
              reinterpret_cast<std::uint8_t*>(current_header_address) +
                  HEADER_SIZE +
                  current_block_size);

      if (available_current_block_address == next_header_address) {
        const std::uint32_t next_block_size = *next_header_address;
        const std::uint32_t new_current_block_size = current_block_size +
            HEADER_SIZE +
            next_block_size;
        *current_header_address = new_current_block_size;

        if (new_current_block_size > *memory_max_block_) {
          memory_max_block_ =
              reinterpret_cast<std::uint32_t*>(current_header_address);
        }

        auto delete_it =
            getIterator<std::set<std::uint32_t*>::iterator>(next_it);
        return getIterator<Iterator>(memory_free_blocks_.erase(delete_it));
      }
    }

    return current_it;
  }

 public:
  std::vector<std::uint8_t*> memory_blocks_;
  std::set<std::uint32_t*> memory_free_blocks_;
  std::uint32_t* memory_max_block_;
};

} // namespace details;

template<std::size_t CHUNK_SIZE = 16'384u>
class CustomAllocationStrategy {

  static_assert(CHUNK_SIZE != 0u, "Chunk size must be more, than zero");
  static_assert(CHUNK_SIZE <= std::numeric_limits<std::uint32_t>::max(),
                "Chunk size must be less or equal max value of the uint32_t");

 public:
  void* allocate(std::size_t size) {

    if (size == 0u) {
      return nullptr;
    }

    for (auto& chunk : memory_chunks_) {
      void* allocated_block = chunk.tryReserveBlock(size);
      if (allocated_block) {
        return allocated_block;
      }
    }

    memory_chunks_.push_back(details::Chunk<CHUNK_SIZE>{});
    auto& chunk = memory_chunks_.back();
    std::uint8_t* allocated_block = chunk.tryReserveBlock(size);
    return allocated_block;
  }

  void deallocate(void* memory_ptr, std::size_t size) {
    if ( (!memory_ptr) || (size == 0u) ) {
      return;
    }

    auto* deallocation_ptr = static_cast<std::uint8_t*>(memory_ptr);
    for (auto& chunk : memory_chunks_) {
      if (chunk.isInside(deallocation_ptr)) {
        chunk.releaseBlock(deallocation_ptr);
      }
    }
  }
 private:
  std::deque<details::Chunk<CHUNK_SIZE>> memory_chunks_{ 1u };

};

template<typename T, std::size_t CHUNK_SIZE = 16'384u>
using CustomAllocator =
    Allocator<T, CustomAllocationStrategy<CHUNK_SIZE>>;

template<typename T>
using CustomAllocatorWithStackChunks =
    Allocator<T, CustomAllocationStrategy<1'024u>>;

template<typename T>
using CustomAllocatorWithHeapChunks =
    Allocator<T, CustomAllocationStrategy<16'384u>>;

template<typename T>
using custom_vector = std::vector<T, CustomAllocator<T>>;

template<typename T>
using custom_list = std::list<T, CustomAllocator<T>>;

template<typename T>
using custom_set = std::set<T, std::less<T>, CustomAllocator<T>>;

template<typename T>
using custom_unordered_set =
    std::unordered_set<T, std::hash<T>, std::equal_to<T>, CustomAllocator<T>>;

using custom_string =
    std::basic_string<char, std::char_traits<char>, CustomAllocator<char>>;

template <typename ValueType>
class Deque_iterator {
 public:
  using iterator_category = std::random_access_iterator_tag;
  using value_type = ValueType;
  using difference_type = std::ptrdiff_t;
  using pointer = ValueType*;
  using reference = ValueType&;

  Deque_iterator() noexcept = default;

  explicit Deque_iterator(pointer p) noexcept : current_(p) {};

  Deque_iterator(const Deque_iterator& other) noexcept :
                              current_(other.current_) {}

  Deque_iterator& operator=(const Deque_iterator& other) {
    this->current_ = other.current_;
    return *this;
  }

  ~Deque_iterator() = default;

  friend void swap(
      Deque_iterator<ValueType>& left, Deque_iterator<ValueType>& right) {
    pointer help = left.current_;
    left.current_ = right.current_;
    right.current_ = help;
  }

  friend bool operator==(const Deque_iterator<ValueType>& left,
                         const Deque_iterator<ValueType>& right) {
    return left.current_ == right.current_;
  }

  friend bool operator!=(const Deque_iterator<ValueType>& left,
                         const Deque_iterator<ValueType>& right) {
    return left.current_ != right.current_;
  }

  reference operator*() const {
    return *current_;
  }

  pointer operator->() const {
    return current_;
  }

  Deque_iterator& operator++() {
    this->current_++;
    return *this;
  }

  Deque_iterator operator++(int) {
    Deque_iterator help = *this;
    ++(*this);
    return help;
  }

  Deque_iterator& operator--() {
    this->current_--;
    return *this;
  }

  Deque_iterator operator--(int) {
    Deque_iterator help = *this;
    --(*this);
    return help;
  }

  Deque_iterator operator+(const difference_type& value) const {
    Deque_iterator help(*(*this) + value);
    return help;
  }

  Deque_iterator& operator+=(const difference_type& value) {
    this->current_ += value;
    return *this;
  }

  Deque_iterator operator-(const difference_type& value) const {
    Deque_iterator help(*(*this) - value);
    return help;
  }

  Deque_iterator& operator-=(const difference_type& value) {
    this->current_ -= value;
    return *this;
  }

  difference_type operator-(const Deque_iterator& other) const {
    return this->current_ - other.current_;
  }

  reference operator[](const difference_type& value) {
    return *(this->current_ + value);
  }

  friend bool operator<(const Deque_iterator<ValueType>& left,
                        const Deque_iterator<ValueType>& right) {
    return left.current_ < right.current_;
  }

  friend bool operator<=(const Deque_iterator<ValueType>& left,
                         const Deque_iterator<ValueType>& right) {
    return left.current_ <= right.current_;
  }

  friend bool operator>(const Deque_iterator<ValueType>& left,
                        const Deque_iterator<ValueType>& right) {
    return left.current_ > right.current_;
  }

  friend bool operator>=(const Deque_iterator<ValueType>& left,
                         const Deque_iterator<ValueType>& right) {
     return left.current_ >= right.current_;
  }

  pointer current_;

};

template <typename ValueType>
class Deque_const_iterator {
  // Shouldn't give non const references on value
 public:
  using iterator_category = std::random_access_iterator_tag;
  using value_type = ValueType;
  using difference_type = std::ptrdiff_t;
  using pointer = const ValueType*;
  using reference = const ValueType&;

  Deque_const_iterator() noexcept = default;

  explicit Deque_const_iterator(pointer p) noexcept : current_(p) {};

  Deque_const_iterator(const Deque_const_iterator& other) noexcept :
                                          current_(other.current_) {}

  explicit Deque_const_iterator(const Deque_iterator<ValueType>& other)
                                  noexcept : current_(other.current_) {}

  Deque_const_iterator& operator=(const Deque_const_iterator& other) {
    this->current_ = other.current_;
    return *this;
  }

  Deque_const_iterator& operator=(const Deque_iterator<ValueType>& other) {
    this->current_ = other.current_;
    return *this;
  }

  ~Deque_const_iterator() = default;

  friend void swap(Deque_const_iterator<ValueType>& left,
                   Deque_const_iterator<ValueType>& right) {
    pointer help = left.current_;
    left.current_ = right.current_;
    right.current_ = help;
  }

  friend bool operator==(const Deque_const_iterator<ValueType>& left,
                         const Deque_const_iterator<ValueType>& right) {
    return left.current_ == right.current_;
  }

  friend bool operator!=(const Deque_const_iterator<ValueType>& left,
                         const Deque_const_iterator<ValueType>& right) {
    return left.current_ != right.current_;
  }

  reference operator*() const {
    return *current_;
  }

  pointer operator->() const {
    return current_;
  }

  Deque_const_iterator& operator++() {
    this->current_++;
    return *this;
  }

  Deque_const_iterator operator++(int) {
    Deque_const_iterator help = *this;
    ++(*this);
    return help;
  }

  Deque_const_iterator& operator--() {
    this->current_--;
    return *this;
  }

  Deque_const_iterator operator--(int) {
    Deque_const_iterator help = *this;
    --(*this);
    return help;
  }

  Deque_const_iterator operator+(const difference_type& value) const {
    Deque_const_iterator help(*(*this) + value);
    return help;
  }

  Deque_const_iterator& operator+=(const difference_type& value) {
    this->current_ += value;
    return *this;
  }

  Deque_const_iterator operator-(const difference_type& value) const {
    Deque_const_iterator help(*(*this) - value);
    return help;
  }

  Deque_const_iterator& operator-=(const difference_type& value) {
    this->current_ -= value;
    return *this;
  }

  difference_type operator-(const Deque_const_iterator& other) const {
    return this->current_ - other.current_;
  }

  reference operator[](const difference_type& value) {
    return *(this->current_ + value);
  }

  friend bool operator<(const Deque_const_iterator<ValueType>& left,
                        const Deque_const_iterator<ValueType>& right) {
    return left.current_ < right.current_;
  }

  friend bool operator<=(const Deque_const_iterator<ValueType>& left,
                         const Deque_const_iterator<ValueType>& right) {
    return left.current_ <= right.current_;
  }

  friend bool operator>(const Deque_const_iterator<ValueType>& left,
                        const Deque_const_iterator<ValueType>& right) {
    return left.current_ > right.current_;
  }

  friend bool operator>=(const Deque_const_iterator<ValueType>& left,
                         const Deque_const_iterator<ValueType>& right) {
    return left.current_ >= right.current_;
  }

  pointer current_;

};

template <class Iter>
class Deque_reverse_iterator {
 public:
  using iterator_type = Iter;
  using iterator_category =
      typename std::iterator_traits<Iter>::iterator_category;
  using value_type = typename std::iterator_traits<Iter>::value_type;
  using difference_type = typename std::iterator_traits<Iter>::difference_type;
  using pointer = typename std::iterator_traits<Iter>::pointer;
  using reference = typename std::iterator_traits<Iter>::reference;

  constexpr Deque_reverse_iterator() = default;

  constexpr explicit Deque_reverse_iterator(
      iterator_type x) : iterator_(x) {}

  template <class U>
  constexpr explicit Deque_reverse_iterator(
      const Deque_reverse_iterator<U>& other) : iterator_(other.base()) {}

  template <class U>
  Deque_reverse_iterator& operator=(const Deque_reverse_iterator<U>& other) {
    this->iterator_ = other.base();
    return *this;
  }

  iterator_type base() const {
    return iterator_;
  }

  reference operator*() const {
    return *iterator_;
  }

  pointer operator->() const {
    return iterator_.current();
  }

  reference operator[](difference_type n) {
    return *(this->iterator_ + n);
  }

  Deque_reverse_iterator& operator++() {
    --this->iterator_;
    return *this;
  }

  Deque_reverse_iterator operator++(int) {
    Deque_reverse_iterator help = *this;
    ++(*this);
    return help;
  }

  Deque_reverse_iterator& operator--() {
    ++this->iterator_;
    return *this;
  }

  Deque_reverse_iterator operator--(int) {
    Deque_reverse_iterator help = *this;
    --(*this);
    return help;
  }

  Deque_reverse_iterator operator+(difference_type n) const {
    Deque_reverse_iterator help(this->iterator_ - n);
    return help;
  }

  Deque_reverse_iterator& operator+=(difference_type n) {
    this->iterator_ -= n;
    return *this;
  }

  Deque_reverse_iterator operator-(difference_type n) const {
    Deque_reverse_iterator help(this->iterator_ + n);
    return help;
  }

  Deque_reverse_iterator& operator-=(difference_type n) {
    this->iterator_ += n;
    return *this;
  }

  template <class Iterator1, class Iterator2>
  friend bool operator==(const Deque_reverse_iterator<Iterator1>& lhs,
                         const Deque_reverse_iterator<Iterator2>& rhs) {
    return lhs.iterator_ == rhs.iterator_;
  }

  template <class Iterator1, class Iterator2>
  friend bool operator!=(const Deque_reverse_iterator<Iterator1>& lhs,
                         const Deque_reverse_iterator<Iterator2>& rhs) {
    return lhs.iterator_ != rhs.iterator_;
  }

  template <class Iterator1, class Iterator2>
  friend bool operator>(const Deque_reverse_iterator<Iterator1>& lhs,
                        const Deque_reverse_iterator<Iterator2>& rhs) {
    return lhs.iterator_ > rhs.iterator_;
  }

  template <class Iterator1, class Iterator2>
  friend bool operator<(const Deque_reverse_iterator<Iterator1>& lhs,
                        const Deque_reverse_iterator<Iterator2>& rhs) {
    return lhs.iterator_ < rhs.iterator_;
  }

  template <class Iterator1, class Iterator2>
  friend bool operator<=(const Deque_reverse_iterator<Iterator1>& lhs,
                         const Deque_reverse_iterator<Iterator2>& rhs) {
    return lhs.iterator_ <= rhs.iterator_;
  }

  template <class Iterator1, class Iterator2>
  friend bool operator>=(const Deque_reverse_iterator<Iterator1>& lhs,
                         const Deque_reverse_iterator<Iterator2>& rhs) {
    return lhs.iterator_ >= rhs.iterator_;
  }

  template <class IterT>
  friend Deque_reverse_iterator<IterT> operator+(
      typename Deque_reverse_iterator<IterT>::difference_type n,
      const Deque_reverse_iterator<IterT>& it) {
    return it + n;
  }

  template <class Iterator>
  friend auto operator-(const Deque_reverse_iterator<Iterator>& lhs,
                        const Deque_reverse_iterator<Iterator>& rhs) {
    return -(lhs.iterator_ - rhs.iterator_);
  }

 private:
  iterator_type iterator_;

};

template <class Iter>
Deque_reverse_iterator<Iter> make_reverse_iterator(Iter i) {
  return Deque_reverse_iterator<Iter>(i);
}

template <
      typename T,
      typename Allocator = Allocator<T, CustomAllocationStrategy<>>
    >
class Deque {
 public:
  using value_type = T;
  using allocator_type = Allocator;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using reference = value_type&;
  using const_reference = const value_type&;
  using pointer = typename std::allocator_traits<Allocator>::pointer;
  using const_pointer = typename std::allocator_traits<Allocator>::const_pointer;
  using iterator = Deque_iterator<value_type>;
  using const_iterator = Deque_const_iterator<value_type>;
  using reverse_iterator = Deque_reverse_iterator<iterator>;
  using const_reverse_iterator = Deque_reverse_iterator<const_iterator>;

  /// @brief Default constructor. Constructs an empty container with a
  /// default-constructed allocator.
  Deque() = default;

  /// @brief Constructs an empty container with the given allocator
  /// @param alloc allocator to use for all memory allocations of this container
  explicit Deque(const Allocator& alloc) : custom_allocator_(alloc) {}

  /// @brief Constructs the container with count copies of elements with value
  /// and with the given allocator
  /// @param count the size of the container
  /// @param value the value to initialize elements of the container with
  /// @param alloc allocator to use for all memory allocations of this container
  Deque(size_type count, const value_type& value, const Allocator& alloc = Allocator())
                                                   : custom_allocator_(alloc) {
    this->size_ = count;
    this->rear_ = this->curr_size_ = this->front_ = 0;
  }

  /// @brief Constructs the container with count default-inserted instances of
  /// T. No copies are made.
  /// @param count the size of the container
  /// @param alloc allocator to use for all memory allocations of this container
  explicit Deque(size_type count, const Allocator& alloc = Allocator())
                                            : custom_allocator_(alloc) {
    this->size_ = count;
    this->rear_ = this->curr_size_ = 0;
  }

  /// @brief Constructs the container with the contents of the range [first,
  /// last).
  /// @tparam InputIt Input Iterator
  /// @param first, last 	the range to copy the elements from
  /// @param alloc allocator to use for all memory allocations of this container
  Deque(iterator first, iterator last, const Allocator& alloc = Allocator())
                                                : custom_allocator_(alloc) {
    while (first != last) {
      this->vector_.push_back(*first++);
    }
    this->curr_size_ = vector_.size();
    this->size_ = vector_.capacity();
    this->front_ = 0;
    this->rear_ = curr_size_;
  }

  /// @brief Copy constructor. Constructs the container with the copy of the
  /// contents of other.
  /// @param other another container to be used as source to initialize the
  /// elements of the container with
  Deque(const Deque& other) {
    *this = other;
  }

  /// @brief Constructs the container with the copy of the contents of other,
  /// using alloc as the allocator.
  /// @param other another container to be used as source to initialize the
  /// elements of the container with
  /// @param alloc allocator to use for all memory allocations of this container
  Deque(const Deque& other, const Allocator& alloc) : custom_allocator_(alloc) {
    *this = other;
  }

  /**
   * @brief Move constructor.
   *
   * Constructs the container with the contents of other using move semantics.
   * Allocator is obtained by move-construction from the allocator belonging to
   * other.
   *
   * @param other another container to be used as source to initialize the
   * elements of the container with
   */
  Deque(Deque&& other)  noexcept {
    *this = other;
    other.clear();
  }

  /**
   * @brief Allocator-extended move constructor.
   * Using alloc as the allocator for the new container, moving the contents
   * from other; if alloc != other.get_allocator(), this results in an
   * element-wise move.
   *
   * @param other another container to be used as source to initialize the
   * elements of the container with
   * @param alloc allocator to use for all memory allocations of this container
   */
  Deque(Deque&& other, const Allocator& alloc) : custom_allocator_(alloc) {
    *this = other;
  }

  /// @brief Constructs the container with the contents of the initializer list
  /// init.
  /// @param init initializer list to initialize the elements of the container
  /// with
  /// @param alloc allocator to use for all memory allocations of this container
  Deque(std::initializer_list<value_type> init, const Allocator& alloc = Allocator()) {
    *this = init;
  }

  /// @brief Destructs the deque.
  ~Deque() {
    this->vector_.clear();
  }

  /// @brief Copy assignment operator. Replaces the contents with a copy of the
  /// contents of other.
  /// @param other another container to use as data source
  /// @return *this
  Deque& operator=(const Deque& other) {
    this->vector_ = other.vector_;
    this->front_ = other.front_;
    this->size_ = other.size_;
    this->rear_ = other.rear_;
    this->curr_size_ = other.curr_size_;\
    return *this;
  }

  /**
   * Move assignment operator.
   *
   * Replaces the contents with those of other using move semantics
   * (i.e. the data in other is moved from other into this container).
   * other is in a valid but unspecified state afterwards.
   *
   * @param other another container to use as data source
   * @return *this
   */
  Deque& operator=(Deque&& other)  noexcept {
    if (this == &other) {
      return *this;
    }
    this->clear();
    this->vector_ = other.vector_;
    this->front_ = other.front_;
    this->size_ = other.size_;
    this->rear_ = other.rear_;
    this->curr_size_ = other.curr_size_;
    other.clear();
    return *this;
  }

  /// @brief Replaces the contents with those identified by initializer list
  /// ilist.
  /// @param ilist
  /// @return this
  Deque& operator=(std::initializer_list<value_type> ilist) {
    this->vector_.resize(ilist.size());
    std::copy(ilist.begin(), ilist.end(), this->vector_);
    this->curr_size_ = vector_.size();
    this->size_ = vector_.capacity();
    this->front_ = 0;
    this->rear_ = curr_size_;
    return *this;
  }

  /// @brief Replaces the contents with count copies of value
  /// @param count
  /// @param value
  void assign(size_type count, const value_type& value) {
    this->clear();
    this->vector_.resize(count, value);
    this->curr_size_ = vector_.size();
    this->size_ = vector_.capacity();
    this->front_ = 0;
    this->rear_ = curr_size_;
  }

  /// @brief Replaces the contents with copies of those in the range [first,
  /// last).
  /// @tparam InputIt
  /// @param first
  /// @param last
  template <class InputIt>
  void assign(InputIt first, InputIt last) {
    this->clear();
    while (first != last) {
      this->vector_.push_back(*first++);
    }
    this->curr_size_ = vector_.size();
    this->size_ = vector_.capacity();
    this->front_ = 0;
    this->rear_ = curr_size_;
  }

  /// @brief Replaces the contents with the elements from the initializer list
  /// ilis
  /// @param ilist
  void assign(std::initializer_list<value_type> ilist) {
    this->clear();
    this->vector_.resize(ilist.size());
    std::copy(ilist.begin(), ilist.end(), this->vector_);
    this->curr_size_ = vector_.size();
    this->size_ = vector_.capacity();
    this->front_ = 0;
    this->rear_ = curr_size_;
  }

  /// @brief Returns the allocator associated with the container.
  /// @return The associated allocator.
  allocator_type get_allocator() const noexcept {
    return custom_allocator_;
  }

  /// ELEMENT ACCESS

  /// @brief Returns a reference to the element at specified location pos, with
  /// bounds checking. If pos is not within the range of the container, an
  /// exception of type std::out_of_range is thrown.
  /// @param pos position of the element to return
  /// @return Reference to the requested element.
  /// @throw std::out_of_range
  reference at(size_type pos) {
    if (pos < 0 || pos > size_) {
      throw std::out_of_range("Out of range)");
    }
    reference at_pos = this->vector_[pos-1];
    return at_pos;
  }

  /// @brief Returns a const reference to the element at specified location pos,
  /// with bounds checking. If pos is not within the range of the container, an
  /// exception of type std::out_of_range is thrown.
  /// @param pos position of the element to return
  /// @return Const Reference to the requested element.
  /// @throw std::out_of_range
  const_reference at(size_type pos) const {
    if (pos < 0 || pos > size_) {
      throw std::out_of_range("Out of range)");
    }
    const_reference at_pos = this->vector_[pos-1];
    return at_pos;
  }

  /// @brief Returns a reference to the element at specified location pos. No
  /// bounds checking is performed.
  /// @param pos position of the element to return
  /// @return Reference to the requested element.
  reference operator[](size_type pos) {
    reference at_pos = this->vector_[pos-1];
    return at_pos;
  }

  /// @brief Returns a const reference to the element at specified location pos.
  /// No bounds checking is performed.
  /// @param pos position of the element to return
  /// @return Const Reference to the requested element.
  const_reference operator[](size_type pos) const {
    const_reference at_pos = this->vector_[pos-1];
    return at_pos;
  }

  /// @brief Returns a reference to the first element in the container.
  /// Calling front on an empty container is undefined.
  /// @return Reference to the first element
  reference front() {
    return this->vector_[front_];
  }

  /// @brief Returns a const reference to the first element in the container.
  /// Calling front on an empty container is undefined.
  /// @return Const reference to the first element
  const_reference front() const {
    return this->vector_[front_];
  }

  /// @brief Returns a reference to the last element in the container.
  /// Calling back on an empty container causes undefined behavior.
  /// @return Reference to the last element.
  reference back() {
    return this->vector_[curr_size_-1];
  }

  /// @brief Returns a const reference to the last element in the container.
  /// Calling back on an empty container causes undefined behavior.
  /// @return Const Reference to the last element.
  const_reference back() const {
    return this->vector_[curr_size_-1];
  }

  /// ITERATORS

  /// @brief Returns an iterator to the first element of the deque.
  /// If the deque is empty, the returned iterator will be equal to end().
  /// @return Iterator to the first element.
  iterator begin() noexcept {
    return iterator(front_);
  }

  /// @brief Returns an iterator to the first element of the deque.
  /// If the deque is empty, the returned iterator will be equal to end().
  /// @return Iterator to the first element.
  const_iterator begin() const noexcept {
    return const_iterator(front_);
  }

  /// @brief Same to begin()
  const_iterator cbegin() const noexcept {
    return const_iterator(front_);
  }

  /// @brief Returns an iterator to the element following the last element of
  /// the deque. This element acts as a placeholder; attempting to access it
  /// results in undefined behavior.
  /// @return Iterator to the element following the last element.
  iterator end() noexcept {
    return iterator(size_);
  }

  /// @brief Returns an constant iterator to the element following the last
  /// element of the deque. This element acts as a placeholder; attempting to
  /// access it results in undefined behavior.
  /// @return Constant Iterator to the element following the last element.
  const_iterator end() const noexcept {
    return const_iterator(size_);
  }

  /// @brief Same to end()
  const_iterator cend() const noexcept {
    return const_iterator(size_);
  }

  /// @brief Returns a reverse iterator to the first element of the reversed
  /// deque. It corresponds to the last element of the non-reversed deque. If
  /// the deque is empty, the returned iterator is equal to rend().
  /// @return Reverse iterator to the first element.
  reverse_iterator rbegin() noexcept {
    return make_reverse_iterator(begin());
  }

  /// @brief Returns a const reverse iterator to the first element of the
  /// reversed deque. It corresponds to the last element of the non-reversed
  /// deque. If the deque is empty, the returned iterator is equal to rend().
  /// @return Const Reverse iterator to the first element.
  const_reverse_iterator rbegin() const noexcept {
    return make_reverse_iterator(cbegin());
  }

  /// @brief Same to rbegin()
  const_reverse_iterator crbegin() const noexcept {
    return make_reverse_iterator(cbegin());
  }

  /// @brief Returns a reverse iterator to the element following the last
  /// element of the reversed deque. It corresponds to the element preceding the
  /// first element of the non-reversed deque. This element acts as a
  /// placeholder, attempting to access it results in undefined behavior.
  /// @return Reverse iterator to the element following the last element.
  reverse_iterator rend() noexcept {
    return make_reverse_iterator(end());
  }

  /// @brief Returns a const reverse iterator to the element following the last
  /// element of the reversed deque. It corresponds to the element preceding the
  /// first element of the non-reversed deque. This element acts as a
  /// placeholder, attempting to access it results in undefined behavior.
  /// @return Const Reverse iterator to the element following the last element.
  const_reverse_iterator rend() const noexcept {
    return make_reverse_iterator(cend());
  }

  /// @brief Same to rend()
  const_reverse_iterator crend() const noexcept {
    return make_reverse_iterator(cend());
  }

  /// CAPACITY

  /// @brief Checks if the container has no elements
  /// @return true if the container is empty, false otherwise
  bool empty() const noexcept {
    return front_ == -1;
  }

  /// @brief Returns the number of elements in the container
  /// @return The number of elements in the container.
  size_type size() const noexcept {
    return curr_size_;
  }

  /// @brief Returns the maximum number of elements the container is able to
  /// hold due to system or library implementation limitations
  /// @return Maximum number of elements.
  size_type max_size() const noexcept {
    return size_;
  }

  /// @brief Requests the removal of unused capacity.
  /// It is a non-binding request to reduce the memory usage without changing
  /// the size of the sequence. All iterators and references are invalidated.
  /// Past-the-end iterator is also invalidated.
  void shrink_to_fit() {
    if (size_ > curr_size_) {
      size_ = curr_size_;
      vector_.resize(size_);
    }
  }

  /// MODIFIERS

  /// @brief Erases all elements from the container.
  /// nvalidates any references, pointers, or iterators referring to contained
  /// elements. Any past-the-end iterators are also invalidated.
  void clear() noexcept {
    this->vector_.clear();
    this->front_ = -1;
    this->rear_ = this->size_ = this->curr_size_ = 0;
  }

  /// @brief Inserts value before pos.
  /// @param pos iterator before which the content will be inserted.
  /// @param value element value to insert
  /// @return Iterator pointing to the inserted value.
  iterator insert(const_iterator pos, const value_type& value) {
    vector_.insert(pos, value);
    this->size_ = vector_.capacity();
    this->curr_size_ = this->rear_ = vector_.size();
  }

  /// @brief Inserts value before pos.
  /// @param pos iterator before which the content will be inserted.
  /// @param value element value to insert
  /// @return Iterator pointing to the inserted value.
  iterator insert(const_iterator pos, value_type&& value) {
    vector_.insert(pos, value);
    this->size_ = vector_.capacity();
    this->curr_size_ = this->rear_ = vector_.size();
  }

  /// @brief Inserts count copies of the value before pos.
  /// @param pos iterator before which the content will be inserted.
  /// @param count number of elements to insert
  /// @param value element value to insert
  /// @return Iterator pointing to the first element inserted, or pos if count
  /// == 0.
  iterator insert(const_iterator pos, size_type count, const value_type& value) {
    vector_.insert(pos, count, value);
    this->size_ = vector_.capacity();
    this->curr_size_ = this->rear_ = vector_.size();
  }

  /// @brief Inserts elements from range [first, last) before pos.
  /// @tparam InputIt Input Iterator
  /// @param pos iterator before which the content will be inserted.
  /// @param first,last the range of elements to insert, can't be iterators into
  /// container for which insert is called
  /// @return Iterator pointing to the first element inserted, or pos if first
  /// == last.
  template <class InputIt>
  iterator insert(const_iterator pos, InputIt first, InputIt last) {
    vector_.insert(pos, first, last);
    this->size_ = vector_.capacity();
    this->curr_size_ = this->rear_ = vector_.size();
  }

  /// @brief Inserts elements from initializer list before pos.
  /// @param pos iterator before which the content will be inserted.
  /// @param ilist initializer list to insert the values from
  /// @return Iterator pointing to the first element inserted, or pos if ilist
  /// is empty.
  iterator insert(const_iterator pos, std::initializer_list<value_type> ilist) {
    vector_.insert(pos, ilist);
    this->size_ = vector_.capacity();
    this->curr_size_ = this->rear_ = vector_.size();
  }

  /// @brief Inserts a new element into the container directly before pos.
  /// @param pos iterator before which the new element will be constructed
  /// @param ...args arguments to forward to the constructor of the element
  /// @return terator pointing to the emplaced element.
  template <class... Args>
  iterator emplace(const_iterator pos, Args&&... args) {
    vector_.emplace(pos, args...);
    this->size_ = vector_.capacity();
    this->curr_size_ = this->rear_ = vector_.size();
  }

  /// @brief Removes the element at pos.
  /// @param pos iterator to the element to remove
  /// @return Iterator following the last removed element.
  iterator erase(const_iterator pos) {
    vector_.erase(pos);
    this->size_ = vector_.capacity();
    this->curr_size_ = this->rear_ = vector_.size();
  }

  /// @brief Removes the elements in the range [first, last).
  /// @param first,last range of elements to remove
  /// @return Iterator following the last removed element.
  iterator erase(const_iterator first, const_iterator last) {
    vector_.erase(first, last);
    this->size_ = vector_.capacity();
    this->curr_size_ = this->rear_ = vector_.size();
  }

  /// @brief Appends the given element value to the end of the container.
  /// The new element is initialized as a copy of value.
  /// @param value the value of the element to append
  void push_back(const value_type& value) {
    insert(cend(), value);
  }

  /// @brief Appends the given element value to the end of the container.
  /// Value is moved into the new element.
  /// @param value the value of the element to append
  void push_back(value_type&& value) {
    insert(cend(), std::move(value));
  }

  /// @brief Appends a new element to the end of the container.
  /// @param ...args arguments to forward to the constructor of the element
  /// @return A reference to the inserted element.
  template <class... Args>
  reference emplace_back(Args&&... args) {
    emplace(cend(), std::forward<Args>(args)...);
  }

  /// @brief Removes the last element of the container.
  void pop_back() {
    erase(cend()-1);
  }

  /// @brief Prepends the given element value to the beginning of the container.
  /// @param value the value of the element to prepend
  void push_front(const value_type& value) {
    insert(cbegin(), value);
  }

  /// @brief Prepends the given element value to the beginning of the container.
  /// @param value moved value of the element to prepend
  void push_front(value_type&& value) {
    insert(cbegin(), std::move(value));
  }

  /// @brief Inserts a new element to the beginning of the container.
  /// @param ...args arguments to forward to the constructor of the element
  /// @return A reference to the inserted element.
  template <class... Args>
  reference emplace_front(Args&&... args) {
    emplace(cbegin(), std::forward<Args>(args)...);
  }

  /// @brief Removes the first element of the container.
  void pop_front() {
    erase(cbegin());
  }

  /// @brief Resizes the container to contain count elements.
  /// If the current size is greater than count, the container is reduced to its
  /// first count elements. If the current size is less than count, additional
  /// default-inserted elements are appended
  /// @param count new size of the container
  void resize(size_type count) {
    vector_.resize(count);
    this->size_ = vector_.capacity();
    this->curr_size_ = this->rear_ = vector_.size();
  }

  /// @brief Resizes the container to contain count elements.
  /// If the current size is greater than count, the container is reduced to its
  /// first count elements. If the current size is less than count, additional
  /// copies of value are appended.
  /// @param count new size of the container
  /// @param value the value to initialize the new elements with
  void resize(size_type count, const value_type& value) {
    vector_.resize(count, value);
    this->size_ = vector_.capacity();
    this->curr_size_ = this->rear_ = vector_.size();
  }

  /// @brief Exchanges the contents of the container with those of other.
  /// Does not invoke any move, copy, or swap operations on individual elements.
  /// All iterators and references remain valid. The past-the-end iterator is
  /// invalidated.
  /// @param other container to exchange the contents with
  void swap(Deque& other) {
    std::swap(this->vector_, other.vector_);
    std::swap(this->rear_, other.rear_);
    std::swap(this->curr_size_, other.curr_size_);
    std::swap(this->size_, other.size_);
    std::swap(this->front_, other.front_);
  }

  /// COMPARISIONS

  /// @brief Checks if the contents of lhs and rhs are equal
  /// @param lhs,rhs deques whose contents to compare
  template <class U, class Alloc>
  friend bool operator==(const Deque<U, Alloc>& lhs,
                         const Deque<U, Alloc>& rhs) {
    return std::equal(lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend());
  }

  /// @brief Checks if the contents of lhs and rhs are not equal
  /// @param lhs,rhs deques whose contents to compare
  template <class U, class Alloc>
  friend bool operator!=(const Deque<U, Alloc>& lhs,
                         const Deque<U, Alloc>& rhs) {
    return lhs.vector_ != rhs.vector_;
  }

  /// @brief Compares the contents of lhs and rhs lexicographically.
  /// @param lhs,rhs deques whose contents to compare
  template <class U, class Alloc>
  friend bool operator>(const Deque<U, Alloc>& lhs, const Deque<U, Alloc>& rhs) {
    return lhs.vector_ > rhs.vector_;
  }

  /// @brief Compares the contents of lhs and rhs lexicographically.
  /// @param lhs,rhs deques whose contents to compare
  template <class U, class Alloc>
  friend bool operator<(const Deque<U, Alloc>& lhs, const Deque<U, Alloc>& rhs) {
    return lhs.vector_ < rhs.vector_;
  }

  /// @brief Compares the contents of lhs and rhs lexicographically.
  /// @param lhs,rhs deques whose contents to compare
  template <class U, class Alloc>
  friend bool operator>=(const Deque<U, Alloc>& lhs,
                         const Deque<U, Alloc>& rhs) {
    return lhs.vector_ >= rhs.vector_;
  }

  /// @brief Compares the contents of lhs and rhs lexicographically.
  /// @param lhs,rhs deques whose contents to compare
  template <class U, class Alloc>
  friend bool operator<=(const Deque<U, Alloc>& lhs,
                         const Deque<U, Alloc>& rhs) {
    return lhs.vector_ <= rhs.vector_;
  }

 private:
  CustomAllocationStrategy<> allocation_area_{};
  CustomAllocator<value_type> custom_allocator_{ allocation_area_ };
  custom_vector<value_type> vector_{ custom_allocator_ };
  int front_{-1};
  size_type rear_{}, size_{}, curr_size_{};

};

/// NON-MEMBER FUNCTIONS

/// @brief  Swaps the contents of lhs and rhs.
/// @param lhs,rhs containers whose contents to swap
template <class T, class Alloc>
void swap(Deque<T, Alloc>& lhs, Deque<T, Alloc>& rhs) {
  lhs.swap(rhs);
}

/// @brief Erases all elements that compare equal to value from the container.
/// @param c container from which to erase
/// @param value value to be removed
/// @return The number of erased elements.
template <class T, class Alloc, class U>
typename Deque<T, Alloc>::size_type erase(Deque<T, Alloc>& c, const U& value) {
  std::size_t count = 0;
  for(std::size_t i = 0; i < c.size(); i++) {
    if (c[i] == value) {
      c.erase(i);
      count++;
    }
  }
  return count;
}

/// @brief Erases all elements that compare equal to value from the container.
/// @param c container from which to erase
/// @param pred unary predicate which returns ​true if the element should be
/// erased.
/// @return The number of erased elements.
template <class T, class Alloc, class Pred>
typename Deque<T, Alloc>::size_type erase_if(Deque<T, Alloc>& c, Pred pred) {
  std::size_t count = 0;
  for(std::size_t i = 0; i < c.size(); i++) {
    if (pred(c[i])) {
      c.erase(i);
      count++;
    }
  }
  return count;
}
}  // namespace fefu_laboratory_two