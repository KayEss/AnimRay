add_executable(ar animray.cpp)
target_link_libraries(ar animray fost-cli)
install(TARGETS ar EXPORT ar RUNTIME DESTINATION bin)
