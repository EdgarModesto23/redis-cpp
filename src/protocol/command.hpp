#pragma once
#include "types.hpp"
#include <memory>

class AbstractCommand {
public:
  virtual std::unique_ptr<types::RESPType> serve() const = 0;
  virtual ~AbstractCommand() = default;
};

class Ping : public AbstractCommand {
public:
  std::unique_ptr<types::RESPType> serve() const override;
  Ping();
};
