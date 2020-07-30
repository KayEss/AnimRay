function(scene name)
    add_executable(${name} ${name}.cpp)
    target_link_libraries(${name} animray fost-cli)
    install(TARGETS ${name} EXPORT ${name} RUNTIME DESTINATION bin)
    add_custom_command(TARGET ${name} POST_BUILD COMMAND ${name})
endfunction()

scene(mix)
