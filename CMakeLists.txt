# This file need to be placed in root directory to work correctly
cmake_minimum_required(VERSION 3.18)

# specify the CXX standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# specify the C standard
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED True)

set(KEYBOARDHOOK_DLL_PATH "${CMAKE_CURRENT_SOURCE_DIR}/DLLs")

project(main)

add_subdirectory(UI)
add_subdirectory(submodules)
