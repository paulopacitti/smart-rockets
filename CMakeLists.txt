cmake_minimum_required (VERSION 2.8.11)
project (SmartRockets)

add_library (SmartRockets smart-rockets.c)
target_include_directories (SmartRockets PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable (smartrocketsexec smart-rockets-exec.c)
target_link_libraries (smartrocketsexec LINK_PUBLIC SmartRockets)