add_executable(mandelbrot mandelbrot.cpp)
target_link_libraries(mandelbrot animray)
install(TARGETS mandelbrot EXPORT mandelbrot RUNTIME DESTINATION bin)
