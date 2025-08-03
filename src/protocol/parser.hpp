#pragma once

#include "command.hpp"
#include <string>
#include <vector>

std::vector<std::string> split (const std::string &s, char delim);

std::vector<std::vector<AbstractCommand>> parse_request(const char* data);
