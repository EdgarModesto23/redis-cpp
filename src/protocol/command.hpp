#pragma once
#include "database.hpp"
#include "types.hpp"
#include <memory>
#include <string>
#include <vector>

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

  Set(std::string key, std::string val, Database &db)
      : key_(key), val_(val), is_expiry_(false), expiry_time_(0), db_(db) {}

  Set(std::string key, std::string val, int expiry, Database &db)
      : key_(key), val_(val), is_expiry_(true), expiry_time_(expiry), db_(db) {}

  static Set New(const std::vector<std::string> &args, Database &db);

private:
  std::string key_;
  std::string val_;
  bool is_expiry_;
  int expiry_time_;
  Database &db_;
};

class Get : public AbstractCommand {
public:
  std::unique_ptr<types::RESPType> serve() const override;
  Get(std::string key, Database &db) : key_(key), db_(db) {}

private:
  Database &db_;
  std::string key_;
};

class ErrorCommand : public AbstractCommand {
public:
  std::unique_ptr<types::RESPType> serve() const override;

  ErrorCommand(const std::string &message) : message_(message) {}

private:
  std::string message_;
};

} // namespace commands
