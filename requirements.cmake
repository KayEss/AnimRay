include(FetchContent)

FetchContent_Declare(
        felspar-exceptions
        GIT_REPOSITORY https://github.com/Felspar/exceptions.git
        GIT_TAG main
    )
FetchContent_Declare(
        felspar-test
        GIT_REPOSITORY https://github.com/Felspar/test.git
        GIT_TAG main
    )

FetchContent_MakeAvailable(felspar-exceptions felspar-test)
