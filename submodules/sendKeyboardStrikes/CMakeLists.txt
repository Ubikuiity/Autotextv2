cmake_minimum_required(VERSION 3.18)

# set the project name and version
project(KeyboardTest)

# specify the C standard
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED True)

add_subdirectory(sources)

add_executable(KeyboardTest tests.c)

target_link_libraries(KeyboardTest PUBLIC sendKeyboardStrikes)
