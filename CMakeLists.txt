cmake_minimum_required(VERSION 3.28)
project(animray)


add_library(animray INTERFACE)
target_include_directories(animray INTERFACE include)
target_compile_features(animray INTERFACE cxx_std_20)
target_link_libraries(animray INTERFACE ${CMAKE_THREAD_LIBS_INIT} felspar-exceptions)


if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    add_custom_target(check)
    include(requirements.cmake)

    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        target_compile_options(animray INTERFACE
                -fconcepts-diagnostics-depth=2
            )
    endif()
endif()


find_package(Threads REQUIRED)


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
