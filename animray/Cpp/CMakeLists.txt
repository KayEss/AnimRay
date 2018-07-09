add_library(animray INTERFACE)
target_include_directories(animray INTERFACE include)
target_compile_features(animray INTERFACE cxx_std_17)

add_subdirectory(animray)
add_subdirectory(animray-test)

