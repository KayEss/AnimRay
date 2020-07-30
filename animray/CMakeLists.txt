cmake_minimum_required(VERSION 3.12)
project(libanimray)

add_library(libanimray INTERFACE)
target_include_directories(libanimray INTERFACE include)
target_compile_features(libanimray INTERFACE cxx_std_20)

add_subdirectory(scenes)
add_subdirectory(tests)

