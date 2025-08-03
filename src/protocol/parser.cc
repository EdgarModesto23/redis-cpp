#include "parser.hpp"
#include <memory>
#include <sstream>
#include <string>

std::vector<std::string> split(const std::string &s, char delim) {
  std::vector<std::string> result;
  std::stringstream ss(s);
  std::string item;

  while (getline(ss, item, delim)) {
    result.push_back(item);
  }

  return result;
}

std::vector<std::unique_ptr<AbstractCommand>>
parse_request(const std::unique_ptr<char> data) {
  std::vector<std::unique_ptr<AbstractCommand>> parsed_request;

  auto commands = split(data.get(), '\n');

  return parsed_request;
};
