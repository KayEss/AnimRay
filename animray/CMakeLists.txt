add_library(animray INTERFACE)
target_include_directories(animray INTERFACE include)
target_compile_features(animray INTERFACE cxx_std_20)

add_subdirectory(scenes)
add_subdirectory(tests)

