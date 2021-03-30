cmake_minimum_required(VERSION 3.12)
project(libanimray)

add_library(animray INTERFACE)
target_include_directories(animray INTERFACE include)
target_compile_features(animray INTERFACE cxx_std_20)
target_link_libraries(animray INTERFACE felspar-exceptions)

add_subdirectory(scenes)
add_subdirectory(tests)

