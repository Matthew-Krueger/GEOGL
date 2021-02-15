set(MODULE_NAME ${PROJECT_NAME}_Engine_${GEOGL_VERSION_FULL})

# Build the DLL
add_library(${MODULE_NAME} SHARED include/GEOGL/Core.hpp Rendering/Window.hpp Application/Application.cpp Utils/MouseCodes.hpp Application/Application.hpp Utils/Headers/PCH.cpp Utils/Headers/PCH.hpp Logging/PrivateLog.hpp Logging/PublicLog.hpp Logging/LoggingFunctions.cpp Win32Exports.hpp include/GEOGL/Core.hpp Utils/Callbacks.cpp Utils/Callbacks.hpp ModelComponents/RawModel.cpp ModelComponents/RawModel.hpp Utils/Loader.cpp Utils/Loader.hpp include/GEOGL/MainCreator.hpp Utils/Headers/Dependencies.hpp Events/Event.hpp Events/ApplicationEvent.hpp Events/KeyEvent.hpp Events/MouseEvent.hpp Utils/KeyCodes.hpp Utils/CallbacksPrivate.hpp Rendering/Layer.cpp Rendering/Layer.hpp Rendering/LayerStack.cpp Rendering/LayerStack.hpp ImGui/ImGuiLayer.cpp ImGui/ImGuiLayer.hpp Rendering/Window.cpp Utils/Settings.cpp Utils/Settings.hpp)

# Include Platforms
add_subdirectory(Platform)

# Optional Platform Libs
set(GEOGL_PLATFORM_LIBS "")
set(GEOGL_OGL_BUILD_FLAG false)
set(GEOGL_VULKAN_BUILD_FLAG false)


if(GEOGL_BUILD_WITH_OPENGL)

    #find OpenGL
    find_library(OpenGL REQUIRED)

    # Set up GEOGL's platform Libs
    set(GEOGL_PLATFORM_LIBS ${GEOGL_PLATFORM_LIBS}GEOGL::Platform::OpenGL)

    # Add OpenGL libs
    if(WIN32)
        set(GEOGL_PLATFORM_LIBS_ADDITIONAL opengl32)
    else()
        set(GEOGL_PLATFORM_LIBS_ADDITIONAL GL)
    endif(WIN32)

    set(GEOGL_OGL_BUILD_FLAG true)

    set(GEOGL_PLATFORM_EXISTS TRUE)
    message("-- Building GEOGL::Engine with OpenGL support.")
endif(GEOGL_BUILD_WITH_OPENGL)
if(GEOGL_BUILD_WITH_VULKAN)
    set(GEOGL_PLATFORM_LIBS ${GEOGL_PLATFORM_LIBS} GEOGL::Platform::Vulkan)
    set(GEOGL_PLATFORM_EXISTS TRUE)
    set(GEOGL_VULKAN_BUILD_FLAG true)
    message("-- Building GEOGL::Engine with Vulkan support.")
endif(GEOGL_BUILD_WITH_VULKAN)

# make sure a platform is selected. If not, error
if (NOT(GEOGL_PLATFORM_EXISTS STREQUAL "TRUE"))
    message(FATAL_ERROR "No platform selected for GEOGL. Please select a platform.")
endif(NOT(GEOGL_PLATFORM_EXISTS STREQUAL "TRUE"))

# Add Dependencies
add_dependencies(${MODULE_NAME} glfw STB_LIB spdlog GLAD ImGui ${GEOGL_PLATFORM_LIBS})
target_link_libraries(${MODULE_NAME} glfw STB_LIB spdlog GLAD ImGui ${GEOGL_PLATFORM_LIBS} ${GEOGL_PLATFORM_LIBS_ADDITIONAL})

target_compile_definitions("${MODULE_NAME}" PRIVATE GEOGL_BUILD_LIBRARY)
target_compile_definitions("${MODULE_NAME}" PRIVATE GEOGL_VERSION_MAJOR=${GEOGL_VERSION_MAJOR})
target_compile_definitions("${MODULE_NAME}" PRIVATE GEOGL_VERSION_MINOR=${GEOGL_VERSION_MINOR})
target_compile_definitions("${MODULE_NAME}" PRIVATE GEOGL_VERSION_PATCH=${GEOGL_VERSION_PATCH})
target_compile_definitions("${MODULE_NAME}" PRIVATE GEOGL_VERSION_FULL="${GEOGL_VERSION_FULL}")
target_compile_definitions("${MODULE_NAME}" PRIVATE GEOGL_ENGINE_NAME="GEOGL-Engine-${GEOGL_VERSION_FULL}")
target_compile_definitions("${MODULE_NAME}" PRIVATE GEOGL_BUILD_WITH_VULKAN=${GEOGL_VULKAN_BUILD_FLAG})
target_compile_definitions("${MODULE_NAME}" PRIVATE GEOGL_BUILD_WITH_OPENGL=${GEOGL_OGL_BUILD_FLAG})

#target_include_directories("${MODULE_NAME}" PUBLIC ${GEOGL_INCLUDE_DIRECTORIES})


target_precompile_headers("${MODULE_NAME}" PRIVATE Utils/Headers/PCH.hpp)

set_target_properties(${MODULE_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/
        )

set_target_properties(${MODULE_NAME} PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/
        )

add_library(GEOGL::Engine ALIAS ${MODULE_NAME})
