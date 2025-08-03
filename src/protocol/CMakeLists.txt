add_library(resp_types types.cc command.cc parser.cc)

target_include_directories(resp_types PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(resp_types PUBLIC spdlog::spdlog_header_only)
