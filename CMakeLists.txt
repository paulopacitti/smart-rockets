cmake_minimum_required (VERSION 2.8.11)
project (SmartRockets)

add_library (SmartRockets smart-rockets.c)

# adding pthreads flag and support
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(SmartRockets PRIVATE Threads::Threads)

target_link_libraries (SmartRockets LINK_PUBLIC m)
target_include_directories (SmartRockets PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable (smartrocketsexec smart-rockets-exec.c)
target_link_libraries (smartrocketsexec LINK_PUBLIC SmartRockets)

set(C_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)