#pragma once
#include "types.hpp"
#include <memory>
#include <string>

namespace commands {
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

class UnknownCommand : public AbstractCommand {
public:
  std::unique_ptr<types::RESPType> serve() const override;
  UnknownCommand(std::string command);

private:
  std::string command_;
};

template <typename T> class ParseRequestError : public AbstractCommand {
public:
  std::unique_ptr<types::RESPType> serve() const override;
  explicit ParseRequestError(T err);

private:
  T error_;
};
} // namespace commands
