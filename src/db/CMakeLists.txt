add_library(database STATIC database.cc)

target_link_libraries(database PRIVATE asio asio::asio)
target_include_directories(database PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

