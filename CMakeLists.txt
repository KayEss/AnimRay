cmake_minimum_required(VERSION 3.10)
project(animray)

enable_testing()

add_subdirectory(Boost)
add_subdirectory(cord)
add_subdirectory(threading)
add_subdirectory(crypto++)
add_subdirectory(fost-base)
add_subdirectory(animray)

target_compile_features(f5-cord INTERFACE cxx_std_17)

