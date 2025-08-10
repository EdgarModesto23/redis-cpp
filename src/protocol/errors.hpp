#pragma once

#include <stdexcept>
#include <string>
class SyntaxError : public std::runtime_error {
private:
  std::string m_;

public:
  explicit SyntaxError(const std::string &message)
      : std::runtime_error(message), m_(message) {}

  std::string message();
};
