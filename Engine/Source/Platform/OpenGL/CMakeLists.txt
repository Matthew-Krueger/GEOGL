######################################
#         Add Vulkan Library         #
######################################
add_library(GEOGL_Platform_OpenGL ${GEOGL_LIBRARY_TYPE}

        ImGuiImpl/imgui_impl_opengl3.h
        ImGuiImpl/imgui_impl_opengl3.cpp
        ImGuiImpl/imgui_impl_glfw.cpp
        ImGuiImpl/imgui_impl_glfw.h

        Rendering/OpenGLGraphicsContext.cpp
        Rendering/OpenGLGraphicsContext.hpp

        Rendering/OpenGLBuffer.cpp
        Rendering/OpenGLBuffer.hpp
        Rendering/OpenGLShader.cpp
        Rendering/OpenGLShader.hpp
        Rendering/OpenGLVertexArray.cpp
        Rendering/OpenGLVertexArray.hpp
        Rendering/OpenGLRendererAPI.cpp
        Rendering/OpenGLRendererAPI.hpp
        Rendering/OpenGLTexture.cpp
        Rendering/OpenGLTexture.hpp)

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
set_target_properties(${GEOGL_LIBRARY_NAME} PROPERTIES POSITION_INDEPENDENT_CODE on)
add_dependencies(${GEOGL_LIBRARY_NAME} GEOGL::Utils glfw GLAD)
target_link_libraries(${GEOGL_LIBRARY_NAME} GEOGL::Utils glfw GLAD ${GEOGL_PLATFORM_LIBS_GL} ${CMAKE_DL_LIBS})

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