add_executable(landmaker landmaker.cpp)
target_link_libraries(landmaker animray fost-cli)
install(TARGETS landmaker EXPORT landmaker RUNTIME DESTINATION bin)

