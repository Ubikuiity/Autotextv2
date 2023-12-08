cmake_minimum_required(VERSION 3.18)

project(main)

# specify the CXX standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# specify the C standard
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED True)

add_executable(main main.cpp)

add_subdirectory(submodules)

target_link_libraries(main PUBLIC caracterReceiver)
target_link_libraries(main PUBLIC logger)
target_link_libraries(main PUBLIC regexMotor)
target_link_libraries(main PUBLIC sendKeyboardStrikes)
target_link_libraries(main PUBLIC yamlHandler)
target_link_libraries(main PUBLIC utils)

# manually import the keyboard Hook DLL
# (For some reason this must be done in project CMake and not in a subdirectory)
add_library(keyboardHook SHARED IMPORTED)
set_property(TARGET keyboardHook PROPERTY IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/DLLs/keyBoardHook.dll")
set_property(TARGET keyboardHook PROPERTY IMPORTED_IMPLIB "${CMAKE_CURRENT_SOURCE_DIR}/DLLs/keyBoardHook.lib")

target_link_libraries(main PUBLIC keyboardHook)
