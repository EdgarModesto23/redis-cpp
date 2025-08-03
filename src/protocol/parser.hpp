#pragma once

#include "command.hpp"
#include <memory.h>
#include <string>
#include <vector>

using namespace commands;

namespace parser {
std::vector<std::unique_ptr<AbstractCommand>>
parse_request(const std::string &data);
} // namespace parser
