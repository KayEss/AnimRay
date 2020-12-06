set(scenes-colour)

macro(scene name)
    add_executable(${name} ${name}.cpp)
    target_link_libraries(${name} animray fost-cli)
    install(TARGETS ${name} EXPORT ${name} RUNTIME DESTINATION bin)
    if(TARGET check)
        add_custom_command(OUTPUT test-${name} COMMAND ${name} -b false)
        list(APPEND scenes-colour test-${name})
    endif()
endmacro()

scene(mix)

if(TARGET check)
    add_custom_target(scenes-colour DEPENDS ${scenes-colour})
    add_dependencies(check scenes-colour)
endif()
