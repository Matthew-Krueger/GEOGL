#######################################################################
#       Configure Optional Dependencies Depending on Build Flags      #
#######################################################################
# Optional Platform Libs
set(GEOGL_OPENGL_BUILD_FLAG 0)
set(GEOGL_VULKAN_BUILD_FLAG 0)
set(GEOGL_PLATFORM_DEPS "")
set(GEOGL_PLATFORM_DEPS_WINDOWING "")

if(BUILD_SHARED_LIBS)
    set(GEOGL_LIBRARY_TYPE SHARED)
    set(GEOGL_MAIN_LIBRARY_TYPE SHARED)
    message("-- Building GEOGL and dependencies as DLLs")
else(BUILD_SHARED_LIBS)
    set(GEOGL_LIBRARY_TYPE OBJECT)
    set(GEOGL_MAIN_LIBRARY_TYPE STATIC)
    message("-- Building GEOGL and dependencies as Object Libraries")
endif(BUILD_SHARED_LIBS)

#######################################################################
#                Configure IGPU Support (Where applicable)            #
#######################################################################
if(GEOGL_ENABLE_IGPU)
    set(GEOGL_ENABLE_IGPU_DEF 1)
else()
    set(GEOGL_ENABLE_IGPU_DEF 0)
endif()

#######################################################################
#                           SELECT PLATFORM                           #
#######################################################################

if(GEOGL_BUILD_WITH_OPENGL OR GEOGL_BUILD_WITH_VULKAN)
    set(GEOGL_GLFW_BUILD_FLAG 1)
    set(GEOGL_PLATFORM_DPES_WINDOWING ${GEOGL_PLATFORM_DEPS_WINDOWING} GEOGL::Platform::GLFW)
endif()

if(GEOGL_BUILD_WITH_OPENGL)
    message("-- Building GEOGL with OpenGL Support")
    set(GEOGL_PLATFORM_EXISTS TRUE)
    set(GEOGL_OPENGL_BUILD_FLAG 1)
    set(GEOGL_PLATFORM_DEPS ${GEOGL_PLATFORM_DEPS} GEOGL::Platform::OpenGL)

endif(GEOGL_BUILD_WITH_OPENGL)
if(GEOGL_BUILD_WITH_VULKAN)

    message("-- Building GEOGL with Vulkan Support")
    set(GEOGL_PLATFORM_EXISTS TRUE)
    set(GEOGL_VULKAN_BUILD_FLAG 1)
    set(GEOGL_PLATFORM_DEPS ${GEOGL_PLATFORM_DEPS} GEOGL::Platform::Vulkan)

endif(GEOGL_BUILD_WITH_VULKAN)



# make sure a platform is selected. If not, error
if (NOT(GEOGL_PLATFORM_EXISTS STREQUAL "TRUE"))
    #message(FATAL_ERROR "No platform selected for GEOGL. Please select a platform.")
endif(NOT(GEOGL_PLATFORM_EXISTS STREQUAL "TRUE"))



#######################################################################
#           Add Dummy target relating to macro definitions            #
#######################################################################
add_library(GEOGL_Interface INTERFACE)
target_compile_definitions("GEOGL_Interface" INTERFACE GEOGL_VERSION_MAJOR=${GEOGL_VERSION_MAJOR})
target_compile_definitions("GEOGL_Interface" INTERFACE GEOGL_VERSION_MINOR=${GEOGL_VERSION_MINOR})
target_compile_definitions("GEOGL_Interface" INTERFACE GEOGL_VERSION_PATCH=${GEOGL_VERSION_PATCH})
target_compile_definitions("GEOGL_Interface" INTERFACE GEOGL_VERSION_FULL="${GEOGL_VERSION_FULL}")
target_compile_definitions("GEOGL_Interface" INTERFACE GEOGL_ENGINE_NAME="GEOGL-Engine-${GEOGL_VERSION_FULL}")
target_compile_definitions("GEOGL_Interface" INTERFACE GEOGL_ENGINE_NAME_NO_VERSION="GEOGL Engine")
target_compile_definitions("GEOGL_Interface" INTERFACE GEOGL_BUILD_WITH_VULKAN=${GEOGL_VULKAN_BUILD_FLAG})
target_compile_definitions("GEOGL_Interface" INTERFACE GEOGL_BUILD_WITH_OPENGL=${GEOGL_OPENGL_BUILD_FLAG})
target_compile_definitions("GEOGL_Interface" INTERFACE GEOGL_ENABLE_IGPU=${GEOGL_ENABLE_IGPU_DEF})
target_compile_definitions("GEOGL_Interface" INTERFACE GEOGL_BUILD_WITH_GLFW=${GEOGL_GLFW_BUILD_FLAG})
if(NOT BUILD_SHARED_LIBS)
    target_compile_definitions("GEOGL_Interface" INTERFACE GEOGL_BUILD_STATIC)
endif(NOT BUILD_SHARED_LIBS)

target_include_directories("GEOGL_Interface" INTERFACE include
        Modules/Platform/OpenGL/include
        Modules/Platform/Vulkan/include
        Modules/Platform/GLFW/include
        Modules/Utils/include
        ../../Dependencies/Nlohmann/JSON/include
        ../../Dependencies/glfw-3.3.2/include
        ../../Dependencies/GLAD/include
        ../../Dependencies/spdlog-1.8.2/include
        ../../Dependencies/STB/include
        ../../Dependencies/GLM
        ../../Dependencies/imgui-docking/include
        )


#target_precompile_headers(GEOGL_Interface INTERFACE Modules/Utils/Headers/PCH.hpp)

#######################################################################
#                      Build all Dependency DLLs                      #
#######################################################################

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

        Rendering/GraphicsContext.cpp Rendering/GraphicsContext.hpp Rendering/Shader.cpp Rendering/Shader.hpp)


# Include Utils And Dependencies
add_subdirectory(Modules/Utils)

# Include Platforms
add_subdirectory(Modules/Platform)

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


target_precompile_headers("${GEOGL_LIBRARY_NAME}" PRIVATE Modules/Utils/Headers/PCH.hpp)

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


