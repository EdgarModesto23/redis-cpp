#include "parser.hpp"
#include "command.hpp"
#include "spdlog/spdlog.h"
#include <memory>
#include <sstream>
#include <string>

namespace parser {

using namespace types;
using namespace commands;

static std::optional<std::string> parse_bulk_string(std::istream &stream) {
  std::string line;
  if (!std::getline(stream, line, '\r') || stream.get() != '\n')
    return std::nullopt;

  int length;
  try {
    length = std::stoi(line);
  } catch (...) {
    return std::nullopt;
  }

  if (length < 0)
    return std::nullopt;

  std::string value(length, '\0');
  stream.read(&value[0], length);
  if (stream.get() != '\r' || stream.get() != '\n')
    return std::nullopt;

  return value;
}

std::vector<std::unique_ptr<AbstractCommand>>
parse_request(const std::string &data) {
  std::vector<std::unique_ptr<AbstractCommand>> commands;
  std::istringstream stream(data);
  spdlog::info("Data to parse: {}", data);
  // Parse Array: starts with '*'
  while (stream.peek() != EOF) {
    if (stream.get() != '*') {
      commands.emplace_back(std::make_unique<ParseRequestError<char>>('?'));
      break;
    }

    std::string line;
    if (!std::getline(stream, line, '\r') || stream.get() != '\n') {
      commands.emplace_back(std::make_unique<ParseRequestError<char>>('?'));
      break;
    }

    int count = std::stoi(line);
    if (count <= 0) {
      commands.emplace_back(std::make_unique<ParseRequestError<char>>('?'));
      break;
    }

    std::vector<std::string> tokens;
    for (int i = 0; i < count; ++i) {
      if (stream.get() != '$') {
        commands.emplace_back(std::make_unique<ParseRequestError<char>>('?'));
        return commands;
      }

      auto bulk = parse_bulk_string(stream);
      if (!bulk.has_value()) {
        commands.emplace_back(std::make_unique<ParseRequestError<char>>('?'));
        return commands;
      }

      tokens.push_back(std::move(bulk.value()));
    }

    if (tokens.empty()) {
      commands.emplace_back(std::make_unique<ParseRequestError<char>>('?'));
      return commands;
    }

    std::string &cmd = tokens[0];
    std::transform(cmd.begin(), cmd.end(), cmd.begin(),
                   [](unsigned char c) { return std::toupper(c); });

    if (cmd == "PING") {
      commands.emplace_back(std::make_unique<Ping>());
    } else {
      commands.emplace_back(std::make_unique<UnknownCommand>(cmd));
    }
  }

  return commands;
}
} // namespace parser
