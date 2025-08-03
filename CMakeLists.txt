cmake_minimum_required(VERSION 3.13)

project(redis-starter-cpp)

set(CMAKE_CXX_STANDARD 23) # Enable the C++23 standard
set(THREADS_PREFER_PTHREAD_FLAG ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Threads REQUIRED)
find_package(asio CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

add_subdirectory(src/protocol)

set(SERVER_SOURCES
  src/main.cc
  src/server.cc
)

add_executable(server ${SERVER_SOURCES})

target_link_libraries(server PRIVATE asio asio::asio)
target_link_libraries(server PRIVATE Threads::Threads)
target_link_libraries(server PRIVATE spdlog::spdlog_header_only)
target_link_libraries(server PRIVATE resp_types)

set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
