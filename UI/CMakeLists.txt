cmake_minimum_required(VERSION 3.18)

set(CMAKE_CXX_STANDARD 17)  # C++17

project(uiMain)

# Crée un executable
add_executable(uiMain WIN32 main.cpp icon.rc)

# Add sources
add_subdirectory(sources)
target_link_libraries(uiMain UISources)

# Add submodules
target_link_libraries(uiMain caracterReceiver)
target_link_libraries(uiMain logger)
target_link_libraries(uiMain regexMotor)
target_link_libraries(uiMain sendKeyboardStrikes)
target_link_libraries(uiMain yamlHandler)
target_link_libraries(uiMain utils)

# manually import the keyboard Hook DLL
# (For some reason this must be done in project CMake and not in a subdirectory)
add_library(keyboardHook SHARED IMPORTED)
set_property(TARGET keyboardHook PROPERTY IMPORTED_LOCATION "${KEYBOARDHOOK_DLL_PATH}/keyBoardHook.dll")
set_property(TARGET keyboardHook PROPERTY IMPORTED_IMPLIB "${KEYBOARDHOOK_DLL_PATH}/keyBoardHook.lib")
target_link_libraries(uiMain keyboardHook)

# Add WxWidgets
# find_package(wxWidgets REQUIRED net gl core base)
find_package(wxWidgets REQUIRED COMPONENTS core base)
include(${wxWidgets_USE_FILE})
target_link_libraries(uiMain ${wxWidgets_LIBRARIES})
 