
add_executable(GEOGL_TESTS TestMain.cpp)

# Link with GEOGL, always test that tests work
add_dependencies(GEOGL_TESTS GEOGL::Engine)
target_link_libraries(GEOGL_TESTS GEOGL::Engine)

add_subdirectory(TestComponents)

set_target_properties(GEOGL_TESTS PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/Tests
        )

add_custom_command(
        TARGET GEOGL_TESTS POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/${PATH_TO_GEOGL}/Engine/Tests/Resources" "${CMAKE_BINARY_DIR}/bin/Tests/Resources"
        COMMENT "Copying resources to bin directory"
)
