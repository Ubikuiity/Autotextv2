# Get all c files in directory
file(GLOB ALL_C_SRCS *.c)

add_library(utils ${ALL_C_SRCS})

target_include_directories(utils PUBLIC headers)
