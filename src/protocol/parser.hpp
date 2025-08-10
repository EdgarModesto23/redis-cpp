#pragma once

#include "command.hpp"
#include <memory.h>
#include <memory>
#include <string>
#include <vector>

using namespace commands;

namespace parser {
std::vector<std::unique_ptr<AbstractCommand>>
parse_request(const std::string &data, Database &db);
} // namespace parser
