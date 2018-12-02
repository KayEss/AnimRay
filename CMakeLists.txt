cmake_minimum_required(VERSION 3.9)
project(animray)

enable_testing()
add_custom_target(check)

add_subdirectory(Boost)
add_subdirectory(cord)
add_subdirectory(threading)
add_subdirectory(crypto++)
add_subdirectory(fost-base)
add_subdirectory(animray)

