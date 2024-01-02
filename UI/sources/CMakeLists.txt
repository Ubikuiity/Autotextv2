# Get all c files in directory
file(GLOB DIR_CPP_SRCS *.cpp)
file(GLOB SUB_PANELS_CPP_SRCS "subPanels/*.cpp")

set(UISOURCES_ALL_SRCS ${DIR_CPP_SRCS} ${SUB_PANELS_CPP_SRCS})  # Forced to create subvariable to send multiple sources to lib

add_library(UISources ${UISOURCES_ALL_SRCS})

# Add WxWidgets library
find_package(wxWidgets REQUIRED net gl core base)
include(${wxWidgets_USE_FILE})

target_include_directories(UISources PUBLIC "./headers")
target_include_directories(UISources PUBLIC "./subPanels/headers")
