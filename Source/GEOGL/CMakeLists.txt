
# Build the GEOGL_DLL
add_library(GEOGL ${GEOGL_MAIN_LIBRARY_TYPE}
        Application/Application.cpp
        Application/Application.hpp

        IO/Events/ApplicationEvent.hpp
        IO/Events/Event.hpp
        IO/Events/KeyEvent.hpp
        IO/Events/MouseEvent.hpp
        IO/Input.cpp
        IO/Input.hpp
        IO/Window.cpp
        IO/Window.hpp

        ImGui/ImGuiLayer.cpp
        ImGui/ImGuiLayer.hpp

        Layers/Layer.cpp
        Layers/Layer.hpp
        Layers/LayerStack.cpp
        Layers/LayerStack.hpp

        include/GEOGL/Core.hpp
        include/GEOGL/MainCreator.hpp

        Rendering/GraphicsContext.cpp
        Rendering/GraphicsContext.hpp
        Rendering/Shader.cpp
        Rendering/Shader.hpp
        Rendering/Buffer.cpp
        Rendering/Buffer.hpp
        Rendering/VertexArray.cpp
        Rendering/VertexArray.hpp
        Rendering/RendererAPI.cpp
        Rendering/RendererAPI.hpp
        Rendering/RenderCommand.hpp
        Rendering/Renderer.cpp
        Rendering/Renderer.hpp
        Rendering/RenderCommand.cpp
        Rendering/Camera.cpp
        Rendering/Camera.hpp)

set(GEOGL_LIBRARY_NAME GEOGL)

# Add Dependencies
add_dependencies(${GEOGL_LIBRARY_NAME} GEOGL::Utils ${GEOGL_PLATFORM_DEPS} ${GEOGL_PLATFORM_DPES_WINDOWING})
target_link_libraries(${GEOGL_LIBRARY_NAME} GEOGL::Utils ${GEOGL_PLATFORM_DEPS} ${GEOGL_PLATFORM_DPES_WINDOWING})

target_include_directories(${GEOGL_LIBRARY_NAME} INTERFACE include
        Modules/Platform/OpenGL/include
        Modules/Platform/Vulkan/include
        Modules/Utils/include
        ../../Dependencies/Nlohmann/JSON/include
        ../../Dependencies/glfw-3.3.2/include
        ../../Dependencies/GLAD/include
        ../../Dependencies/spdlog-1.8.2/include
        ../../Dependencies/STB/include
        ../../Dependencies/GLM
        ../../Dependencies/imgui-docking/include
        )


target_precompile_headers("${GEOGL_LIBRARY_NAME}" PRIVATE ../Utils/Headers/PCH.hpp)

set_target_properties(${GEOGL_LIBRARY_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/
        )

set_target_properties(${GEOGL_LIBRARY_NAME} PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/
        )

set_target_properties(${GEOGL_LIBRARY_NAME} PROPERTIES POSITION_INDEPENDENT_CODE on)

######################################
#  PostBuild Copy VK_LAYER_SETTINGS  #
######################################
if(GEOGL_BUILD_WITH_VULKAN)
    add_custom_command(
            TARGET ${GEOGL_LIBRARY_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/${PATH_TO_GEOGL}/Source/GEOGL/Modules/Platform/Vulkan/vk_layer_settings.txt" "${CMAKE_BINARY_DIR}/bin/vk_layer_settings.txt"
            COMMENT "Copying vk_layer_settings.txt to build directory"
    )
endif()

add_library(GEOGL::Engine ALIAS ${GEOGL_LIBRARY_NAME})


