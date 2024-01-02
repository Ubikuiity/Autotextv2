cmake_minimum_required(VERSION 3.18)

set(CMAKE_CXX_STANDARD 17)  # C++17

project(main)

# Crée un executable
add_executable(main WIN32 main.cpp)

# Add sources
add_subdirectory(sources)
target_link_libraries(main UISources)

# Add WxWidgets
find_package(wxWidgets REQUIRED net gl core base)
include(${wxWidgets_USE_FILE})
target_link_libraries(main ${wxWidgets_LIBRARIES})
 