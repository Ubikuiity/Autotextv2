# Get all c files in directory
file(GLOB ALL_C_SRCS *.c)

add_library(yamlHandler ${ALL_C_SRCS})

target_include_directories(yamlHandler PUBLIC headers)
