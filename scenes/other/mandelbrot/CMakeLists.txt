add_executable(mandelbrot mandelbrot.cpp)
target_link_libraries(mandelbrot animray fost-cli)
install(TARGETS mandelbrot EXPORT mandelbrot RUNTIME DESTINATION bin)
