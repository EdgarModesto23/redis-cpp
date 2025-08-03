#pragma once

#include <string>
#include <vector>
namespace types {

enum ERROR_TYPE { ERR, WRONGTYPE };

class RESPType {
public:
  virtual std::vector<char> to_bytes() const = 0;
  virtual ~RESPType() = default;
};

class ErrorString : public RESPType {
private:
  std::string data_;
  ERROR_TYPE type_;

public:
  std::vector<char> to_bytes() const override;
  ErrorString(std::string data, ERROR_TYPE type) : data_(data), type_(type) {}
};

class SimpleString : public RESPType {
private:
  std::string data_;

public:
  std::vector<char> to_bytes() const override;
  SimpleString(std::string data) : data_(data) {}
};

class BulkString : public RESPType {
private:
  std::string data_;

public:
  std::vector<char> to_bytes() const override;
  BulkString(std::string data) : data_(data) {}
};

} // namespace types
