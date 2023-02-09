#pragma once

#include <vector>
#include <utility>


template<typename Tp>
class FixedAllocator {
  PageAllocator page_allocator_;

 private:
  std::vector<std::pair<Tp *, uint64_t>> free_memory;
  uint64_t page_size;

 public:
  explicit FixedAllocator(std::uint64_t page_size_c)
      : page_allocator_(page_size_c) {
    page_size = page_size_c;
  }

  Tp *Allocate() {
    for (uint64_t i = 0; i < free_memory.size(); i++) {
      if (free_memory[i].second < page_size) {
        free_memory[i].second++;
        return free_memory[i].first;
      }
    }
    free_memory.emplace_back(static_cast<Tp *>(page_allocator_.Allocate()), 1);
    return free_memory.back().first;
  }

  void Deallocate(Tp *p) {
    auto ind = Search_Index(p);
    if (ind != -1) {
      free_memory[ind].second--;
    }
  }

  const PageAllocator &InnerAllocator() const noexcept {
    return page_allocator_;
  }

  int64_t Search_Index(Tp *p) {
    for (uint64_t i = 0; i < free_memory.size(); i++)
      if (free_memory[i].first == p)
        return i;
    return -1;
  }
};
