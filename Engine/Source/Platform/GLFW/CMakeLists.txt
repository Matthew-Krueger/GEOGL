#######################################
#         Add GLFW Library            #
#######################################
add_library(GEOGL_GLFW ${GEOGL_LIBRARY_TYPE}

        IO/GLFWInput.hpp
        IO/GLFWInput.cpp
        IO/GLFWKeyCodes.hpp
        IO/GLFWKeyCodes.cpp
        IO/GLFWWindow.hpp
        IO/GLFWWindow.cpp

        include/GEOGL/Platform/GLFW.hpp

        )

######################################
#     Set name for use elsewhere     #
######################################
set(GEOGL_LIBRARY_NAME GEOGL_GLFW)

######################################
#         Linker Dependencies        #
######################################
# Utils, glfw, GLAD
set_target_properties(${GEOGL_LIBRARY_NAME} PROPERTIES POSITION_INDEPENDENT_CODE on)
add_dependencies(${GEOGL_LIBRARY_NAME} GEOGL::Utils glfw ${GEOGL_PLATFORM_DEPS})
target_link_libraries(${GEOGL_LIBRARY_NAME} GEOGL::Utils glfw ${GEOGL_PLATFORM_DEPS})

######################################
#      PreProcessor Definitions      #
######################################
target_compile_definitions("${GEOGL_LIBRARY_NAME}" PRIVATE GEOGL_BUILD_LIBRARY)

######################################
#         Precompiled Headers        #
######################################
target_precompile_headers("${GEOGL_LIBRARY_NAME}" PRIVATE ../../Utils/Headers/PCH.hpp)

######################################
#         Emit to bin Folder         #
######################################
set_target_properties("${GEOGL_LIBRARY_NAME}" PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/
        )

set_target_properties("${GEOGL_LIBRARY_NAME}" PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/
        )

######################################
#     Add Alias target for looks     #
######################################
add_library(GEOGL::Platform::GLFW ALIAS ${GEOGL_LIBRARY_NAME})