set(scenes-colour)

macro(scene name output)
    add_executable(${name} ${name}.cpp)
    target_link_libraries(${name} animray)
    install(TARGETS ${name} EXPORT ${name} RUNTIME DESTINATION bin)
    if(TARGET check)
        add_custom_command(OUTPUT ${output}
                COMMAND ${name} -b false
                DEPENDS ${name})
        list(APPEND scenes-colour ${output})
    endif()
endmacro()

scene(mix mix-linear.tga)

if(TARGET check)
    add_custom_target(scenes-colour DEPENDS ${scenes-colour})
    add_dependencies(check scenes-colour)
endif()
