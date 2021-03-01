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

add_executable(GEOGL_Example ${OPTIONS} ExampleApplication.cpp ExampleApplication.hpp ${ADDITIONAL_WIN32_FILES} 3DLayer.cpp 3DLayer.hpp PCH/PCH.cpp PCH/PCH.hpp)

target_precompile_headers("GEOGL_Example" PRIVATE PCH/PCH.hpp)

add_dependencies(GEOGL_Example GEOGL::Engine)
target_link_libraries(GEOGL_Example GEOGL::Engine)

set_target_properties(GEOGL_Example PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/
        )



add_custom_command(
        TARGET GEOGL_Example POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/${PATH_TO_GEOGL}/Source/Example/Resources" "${CMAKE_BINARY_DIR}/bin/Resources"
        COMMENT "Copying resources to bin directory"
)