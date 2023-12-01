cmake_minimum_required(VERSION 3.18)

project(loggerTest)

set(CMAKE_CXX_STANDARD 17)

add_executable(loggerTest tests.cpp)

add_subdirectory(sources)

target_link_libraries(loggerTest logger)
