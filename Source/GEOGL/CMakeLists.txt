set(MODULE_NAME ${PROJECT_NAME}_Engine_${GEOGL_VERSION_FULL})


#######################################################################
#       Configure Optional Dependencies Depending on Build Flags      #
#######################################################################
# Optional Platform Libs
set(GEOGL_PLATFORM_LIBS "")
set(GEOGL_OGL_BUILD_FLAG 0)
set(GEOGL_VULKAN_BUILD_FLAG 0)


if(GEOGL_BUILD_WITH_OPENGL)

    #find OpenGL
    find_library(OpenGL REQUIRED)

    # Set up GEOGL's platform Libs
    set(GEOGL_PLATFORM_LIBS ${GEOGL_PLATFORM_LIBS}GEOGL::Platform::OpenGL GLAD)

    # Add OpenGL libs
    if(WIN32)
        set(GEOGL_PLATFORM_LIBS_ADDITIONAL opengl32)
    else()
        set(GEOGL_PLATFORM_LIBS_ADDITIONAL GL)
    endif(WIN32)

    set(GEOGL_OGL_BUILD_FLAG 1)

    set(GEOGL_PLATFORM_EXISTS TRUE)
    message("-- Building GEOGL::Engine with OpenGL support.")
endif(GEOGL_BUILD_WITH_OPENGL)
if(GEOGL_BUILD_WITH_VULKAN)
    set(GEOGL_PLATFORM_LIBS ${GEOGL_PLATFORM_LIBS} GEOGL::Platform::Vulkan)
    set(GEOGL_PLATFORM_EXISTS TRUE)
    set(GEOGL_VULKAN_BUILD_FLAG 1)
    set(GEOGL_PLATFORM_LIBS_ADDITIONAL ${VULKAN_LIB_LIST})
    message("-- Building GEOGL::Engine with Vulkan support.")
endif(GEOGL_BUILD_WITH_VULKAN)


#######################################################################
#                      Build all Dependency DLLs                      #
#######################################################################

# Include Utils And Dependencies
add_subdirectory(Utils)

# Include Platforms
add_subdirectory(Platform)

# Build the GEOGL_DLL
add_library(${MODULE_NAME} SHARED
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

        Rendering/Layer.cpp
        Rendering/Layer.hpp
        Rendering/LayerStack.cpp
        Rendering/LayerStack.hpp

        Utils/Win32Exports.hpp

        include/GEOGL/Core.hpp
        include/GEOGL/MainCreator.hpp
        )

# make sure a platform is selected. If not, error
if (NOT(GEOGL_PLATFORM_EXISTS STREQUAL "TRUE"))
    message(FATAL_ERROR "No platform selected for GEOGL. Please select a platform.")
endif(NOT(GEOGL_PLATFORM_EXISTS STREQUAL "TRUE"))

# Add Dependencies
add_dependencies(${MODULE_NAME} GEOGL::Utils ${GEOGL_PLATFORM_LIBS})
target_link_libraries(${MODULE_NAME} GEOGL_Utils ${GEOGL_PLATFORM_LIBS} ${GEOGL_PLATFORM_LIBS_ADDITIONAL})

#target_include_directories("${MODULE_NAME}" PUBLIC ${GEOGL_INCLUDE_DIRECTORIES})


target_precompile_headers("${MODULE_NAME}" PRIVATE Utils/Headers/PCH.hpp)

set_target_properties(${MODULE_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/
        )

set_target_properties(${MODULE_NAME} PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/
        )

add_library(GEOGL::Engine ALIAS ${MODULE_NAME})
