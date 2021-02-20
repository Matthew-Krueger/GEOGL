######################################
#         Add Vulkan Library         #
######################################
add_library(GEOGL_Platform_OpenGL ${GEOGL_LIBRARY_TYPE}

        IO/Window.cpp
        IO/Window.hpp
        IO/Input.hpp
        IO/Input.cpp
        IO/KeyCodes.cpp
        IO/KeyCodes.hpp

        ImGuiImpl/imgui_impl_opengl3.h
        ImGuiImpl/imgui_impl_opengl3.cpp
        ImGuiImpl/imgui_impl_glfw.cpp
        ImGuiImpl/imgui_impl_glfw.h

        )
######################################
#     Set name for use elsewhere     #
######################################
set(GEOGL_LIBRARY_NAME GEOGL_Platform_OpenGL)


if(WIN32)
    set(GEOGL_PLATFORM_LIBS_GL opengl32)
else()
    set(GEOGL_PLATFORM_LIBS_GL GL)
endif(WIN32)

######################################
#         Linker Dependencies        #
######################################
# Utils, glfw, GLAD
add_dependencies(${GEOGL_LIBRARY_NAME} GEOGL::Utils glfw GLAD)
target_link_libraries(${GEOGL_LIBRARY_NAME} GEOGL::Utils glfw GLAD ${GEOGL_PLATFORM_LIBS_GL})

######################################
#      Parse if IGPU is allowed      #
######################################
set(GEOGL_ENABLE_IGPU_DEF 0)
if(GEOGL_ENABLE_IGPU)
    set(GEOGL_ENABLE_IGPU_DEF 1)
endif()

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
add_library(GEOGL::Platform::OpenGL ALIAS ${GEOGL_LIBRARY_NAME})