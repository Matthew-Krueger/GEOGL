if(WIN32)
    #set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} /link /SUBSYSTEM:WINDOWS /entry:main)
    #message(${CMAKE_BUILD_TYPE})
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        message("-- Debug Mode For Win32")
    else(CMAKE_BUILD_TYPE STREQUAL "Debug")
        message("-- Release Mode for Win32")
        SET(OPTIONS WIN32)
    endif(CMAKE_BUILD_TYPE STREQUAL "Debug")

    set(ADDITIONAL_WIN32_FILES Icons/imageIcon.rc EditorLayer.cpp EditorLayer.hpp)

endif(WIN32)

add_executable(GEOGL-Editor ${OPTIONS} ${ADDITIONAL_WIN32_FILES}
        PCH/PCH.cpp PCH/PCH.hpp
        GEOGLEditorApp.cpp GEOGLEditorApp.hpp)

target_precompile_headers(GEOGL-Editor PRIVATE PCH/PCH.hpp)

add_dependencies(GEOGL-Editor GEOGL::Engine)
target_link_libraries(GEOGL-Editor GEOGL::Engine)

set_target_properties(GEOGL-Editor PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
        )

add_custom_command(
        TARGET GEOGL-Editor POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/${PATH_TO_GEOGL}/Projects/GEOGL-Editor/Resources" "${CMAKE_BINARY_DIR}/bin/GEOGL-Editor-Resources"
        COMMENT "Copying resources to bin directory"
)
