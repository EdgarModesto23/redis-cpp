#pragma once

#include <string>
#include <vector>
namespace types {
class RESPType {
public:
  virtual std::vector<char> to_bytes() const = 0;
  virtual ~RESPType() = default;
};

class SimpleString : public RESPType {
private:
  std::string data_;

public:
  std::vector<char> to_bytes() const override;
  SimpleString(std::string data) : data_(data) {};
};
} // namespace types
