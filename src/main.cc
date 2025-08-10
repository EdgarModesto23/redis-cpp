#include "server.hpp"
#include <spdlog/common.h>
#include <spdlog/spdlog.h>

auto main(int argc, char **argv) -> int {
  spdlog::set_level(spdlog::level::debug);
  init_server();
}
