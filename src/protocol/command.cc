#include "command.hpp"
#include "types.hpp"
#include <format>
#include <memory>
#include <string>

namespace commands {
std::unique_ptr<types::RESPType> commands::Ping::serve() const {
  return std::make_unique<types::SimpleString>(types::SimpleString("PONG"));
}

std::unique_ptr<types::RESPType> commands::UnknownCommand::serve() const {
  return std::make_unique<types::ErrorString>(
      std::format("Unknown command {}\n", command_), types::WRONGTYPE);
}

commands::UnknownCommand::UnknownCommand(std::string command)
    : command_(command) {};

commands::Ping::Ping() {};

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

} // namespace commands
