#pragma once
#include "types.hpp"

class AbstractCommand {
public:
  virtual RESPType serve() const = 0;
  virtual ~AbstractCommand() = default;
};

class Ping {};
