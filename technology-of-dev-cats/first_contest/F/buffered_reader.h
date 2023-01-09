#include <algorithm>
#include <string>
#include <utility>

class PackageStreaM {
 public:
  PackageStreaM(std::string data, int32_t package_len)
      : package_len_(package_len), data_(std::move(data)) {}

  int32_t PackageLen() const { return package_len_; }

  int32_t ReadPackage(char* buf) {
    int32_t next_pos = min<int32_t>(data_.size(), pos_ + PackageLen());
    int32_t len = next_pos - pos_;
    memcpy(buf, data_.c_str() + pos_, len);
    pos_ = next_pos;
    return len;
  }

 public:
  const int32_t package_len_;
  int32_t pos_ = 0;
  std::string data_;
};

class BufferedReader {
  PackageStream* str;
  int32_t p = 0;

 public:
  explicit BufferedReader(PackageStream* stream) : str(stream) {}

  int32_t Read(char* output_buffer, int32_t buffer_len) {
    int32_t len;
    len = min(static_cast<int32_t>(
                  reinterpret_cast<PackageStreaM*>(str)->data_.length()) -
                  BufferedReader::p,
              buffer_len);
    memcpy(output_buffer,
           reinterpret_cast<PackageStreaM*>(str)->data_.c_str()
           + BufferedReader::p, len);
    BufferedReader::p += len;
    return len;
  }
};
