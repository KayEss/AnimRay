cmake_minimum_required(VERSION 3.12)
project(libanimray)


if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    add_custom_target(check)
    include(requirements.cmake)
endif()


find_package(Threads REQUIRED)

add_library(animray INTERFACE)
target_include_directories(animray INTERFACE include)
target_compile_features(animray INTERFACE cxx_std_20)
target_link_libraries(animray INTERFACE ${CMAKE_THREAD_LIBS_INIT} felspar-exceptions)
if(${CUDA_PATH})
    target_compile_definitions(animray INTERFACE CUDA_BUILD)
    target_link_options(animray INTERFACE
            -xcuda --cuda-path=${CUDA_PATH}
            --cuda-gpu-arch=sm_50
        )
endif()


if(${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
    target_compile_definitions(animray INTERFACE NO_CONCEPTS_HEADER)
endif()

if(TARGET check)
    add_subdirectory(scenes)
    add_subdirectory(tests)
endif()
