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

add_executable(GEOGL_Example_Vulkan ${OPTIONS} ${ADDITIONAL_WIN32_FILES} VulkanExampleApp.cpp VulkanExampleApp.hpp PCH/PCH.cpp PCH/PCH.hpp)

target_precompile_headers("GEOGL_Example_Vulkan" PRIVATE PCH/PCH.hpp)

add_dependencies(GEOGL_Example_Vulkan GEOGL::Engine)
target_link_libraries(GEOGL_Example_Vulkan GEOGL::Engine)

set_target_properties(GEOGL_Example_Vulkan PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/
        )



add_custom_command(
        TARGET GEOGL_Example_Vulkan POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/${PATH_TO_GEOGL}/Source/Example/Resources" "${CMAKE_BINARY_DIR}/bin/Resources"
        COMMENT "Copying resources to bin directory"
)