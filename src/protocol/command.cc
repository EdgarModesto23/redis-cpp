#include "command.hpp"
#include "types.hpp"
#include <memory>

std::unique_ptr<types::RESPType> Ping::serve() const {
  return std::make_unique<types::SimpleString>(
      types::SimpleString("+PONG\r\n"));
}
