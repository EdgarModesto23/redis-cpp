#include "command.hpp"
#include "database.hpp"
#include "types.hpp"
#include <format>
#include <memory>
#include <string>

namespace commands {

std::unique_ptr<types::RESPType> commands::Ping::serve() const {
  return std::make_unique<types::SimpleString>("PONG");
}

commands::Ping::Ping() {};

std::unique_ptr<types::RESPType> commands::UnknownCommand::serve() const {
  return std::make_unique<types::ErrorString>(
      std::format("Unknown command {}\n", command_), types::WRONGTYPE);
}

commands::UnknownCommand::UnknownCommand(std::string command)
    : command_(command) {};

template <typename T>
commands::ParseRequestError<T>::ParseRequestError(T err) : error_(err){};

template <>
std::unique_ptr<types::RESPType>
commands::ParseRequestError<char>::serve() const {
  return std::make_unique<types::ErrorString>(
      std::format("Expected Array of commands (*), got: {} instead\n", error_),
      types::WRONGTYPE);
}

template <>
commands::ParseRequestError<char>::ParseRequestError(char err) : error_(err){};

commands::Echo::Echo(std::string message) : message_(message) {};

std::unique_ptr<types::RESPType> commands::Echo::serve() const {
  return std::make_unique<types::BulkString>(types::BulkString(message_));
}

std::unique_ptr<types::RESPType> commands::Set::serve() const {
  db.add(key_, val_);
  return std::make_unique<types::SimpleString>("OK");
}

std::unique_ptr<types::RESPType> commands::Get::serve() const {
  auto value = db.get<std::string>(key_);
  return std::make_unique<types::BulkString>(value);
}

} // namespace commands
