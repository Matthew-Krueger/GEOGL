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

add_executable(GEOGL_Example2DGame ${OPTIONS} ${ADDITIONAL_WIN32_FILES} PCH/PCH.cpp PCH/PCH.hpp 2DGame.cpp 2DGame.hpp DebugLayer.cpp DebugLayer.hpp GameLayer.cpp GameLayer.hpp)

target_precompile_headers(GEOGL_Example2DGame PRIVATE PCH/PCH.hpp)

add_dependencies(GEOGL_Example2DGame GEOGL::Engine)
target_link_libraries(GEOGL_Example2DGame GEOGL::Engine)

set_target_properties(GEOGL_Example2DGame PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
        )

add_custom_command(
        TARGET GEOGL_Example2DGame POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/${PATH_TO_GEOGL}/Projects/Example2DGame/Resources" "${CMAKE_BINARY_DIR}/bin/Example2DGameResources"
        COMMENT "Copying resources to bin directory"
)
