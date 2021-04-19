if(${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
    add_compile_options(
            -Werror
            -fdiagnostics-show-option
            -Wall -Wextra -Wpedantic

            # Turn these off because they're safe and do exactly what we want
            -Wno-missing-field-initializers
        )
endif()

add_test_run(check animray TESTS
        animation-animate-tests.cpp
        animation-procedural-tests.cpp
        colour-hsl-tests.cpp
        colour-rgba-tests.cpp
        colour-rgb-tests.cpp
        extents2d-tests.cpp
        film-tests.cpp
        functional-callable-tests.cpp
        geometry-plane-tests.cpp
        geometry-sphere-tests.cpp
        geometry-triangle-tests.cpp
        interpolation-linear-tests.cpp
        line-tests.cpp
        maths-cross-tests.cpp
        maths-matrix-tests.cpp
        maths-prime-tests.cpp
        mixins-tests.cpp
        numeric.tests.cpp
        point2d-tests.cpp
        point3d-tests.cpp
        ray-tests.cpp
        surface-tests.cpp
        texture-tests.cpp
        unit-vector-tests.cpp
    )
