add_library(caracterReceiver caracterReceiver.c)

target_include_directories(caracterReceiver PUBLIC headers)

# Linking the WS_32 library for the windows network library
target_link_libraries(caracterReceiver WS2_32)
