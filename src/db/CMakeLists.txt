add_library(database STATIC database.cc)

target_include_directories(database PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

