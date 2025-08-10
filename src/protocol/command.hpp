#pragma once
#include "database.hpp"
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

class Echo : public AbstractCommand {
public:
  std::unique_ptr<types::RESPType> serve() const override;
  Echo(std::string message);

private:
  std::string message_;
};

template <typename T> class ParseRequestError : public AbstractCommand {
public:
  std::unique_ptr<types::RESPType> serve() const override;
  explicit ParseRequestError(T err);

private:
  T error_;
};

class Set : public AbstractCommand {
public:
  std::unique_ptr<types::RESPType> serve() const override;

  Set(std::string key, std::string val) : key_(key), val_(val) {}

private:
  std::string key_;
  std::string val_;
};

class Get : public AbstractCommand {
public:
  std::unique_ptr<types::RESPType> serve() const override;
  Get(std::string key) : key_(key) {}

private:
  std::string key_;
};

} // namespace commands
