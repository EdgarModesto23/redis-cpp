#include "types.hpp"
#include <vector>

std::vector<char> types::SimpleString::to_bytes() const {
  std::vector<char> result;
  result.push_back('+');
  result.insert(result.end(), data_.begin(), data_.end());
  result.push_back('\r');
  result.push_back('\n');
  return result;
}
