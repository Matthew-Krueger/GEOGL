if(WIN32)
    #set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} /link /SUBSYSTEM:WINDOWS /entry:main)
    #message(${CMAKE_BUILD_TYPE})
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        message("-- Debug Mode For Win32")
    else(CMAKE_BUILD_TYPE STREQUAL "Debug")
        message("-- Release Mode for Win32")
        SET(OPTIONS WIN32)
    endif(CMAKE_BUILD_TYPE STREQUAL "Debug")

    set(ADDITIONAL_WIN32_FILES Icons/imageIcon.rc)

endif(WIN32)

add_executable(GEOGL_SandboxApp ${OPTIONS} ${ADDITIONAL_WIN32_FILES} SandboxApp.cpp SandboxApp.hpp Layer2D.cpp Layer2D.hpp DebugLayer.cpp DebugLayer.hpp ParticleSystem.cpp ParticleSystem.hpp)

add_dependencies(GEOGL_SandboxApp GEOGL::Engine)
target_link_libraries(GEOGL_SandboxApp GEOGL::Engine)

set_target_properties(GEOGL_SandboxApp PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/Sandbox
        )

add_custom_command(
        TARGET GEOGL_SandboxApp POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/${PATH_TO_GEOGL}/Projects/Sandbox/Resources" "${CMAKE_BINARY_DIR}/bin/Sandbox/Resources"
        COMMENT "Copying resources to bin directory"
)

