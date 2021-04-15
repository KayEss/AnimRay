add_executable(landmaker landmaker.cpp)
target_link_libraries(landmaker animray)
install(TARGETS landmaker EXPORT landmaker RUNTIME DESTINATION bin)

