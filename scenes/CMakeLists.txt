if(${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
    add_compile_options(
            -Werror
            -fdiagnostics-show-option
            -Wall -Wextra -Wpedantic

            # Turn these off because they're safe and do exactly what we want
            -Wno-missing-field-initializers
        )
endif()

add_subdirectory(colour)
add_subdirectory(early)
add_subdirectory(other)

