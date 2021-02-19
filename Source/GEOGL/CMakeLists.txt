#######################################################################
#       Configure Optional Dependencies Depending on Build Flags      #
#######################################################################
# Optional Platform Libs
set(GEOGL_PLATFORM_LIBS "")
set(GEOGL_GENERATOR_LIBS "")
set(GEOGL_OGL_BUILD_FLAG 0)
set(GEOGL_VULKAN_BUILD_FLAG 0)

if(BUILD_SHARED_LIBS)
    set(GEOGL_LIBRARY_TYPE SHARED)
    message("-- Building GEOGL and dependencies as DLLs")
else(BUILD_SHARED_LIBS)
    set(GEOGL_LIBRARY_TYPE OBJECT)
    message("-- Building GEOGL and dependencies as Object Libraries")
endif(BUILD_SHARED_LIBS)


if(GEOGL_BUILD_WITH_OPENGL)

    #find OpenGL
    find_library(OpenGL REQUIRED)

    # Set up GEOGL's platform Libs
    set(GEOGL_PLATFORM_LIBS ${GEOGL_PLATFORM_LIBS}GEOGL::Platform::OpenGL)
    set(GEOGL_GENERATOR_LIBS "${GEOGL_GENERATOR_LIBS}$<TARGET_OBJECTS:GEOGL::Platform::OpenGL>")

    set(GEOGL_OGL_BUILD_FLAG 1)

    set(GEOGL_PLATFORM_EXISTS TRUE)
    message("-- Building GEOGL::Engine with OpenGL support.")
endif(GEOGL_BUILD_WITH_OPENGL)
if(GEOGL_BUILD_WITH_VULKAN)
    set(GEOGL_PLATFORM_LIBS ${GEOGL_PLATFORM_LIBS} GEOGL::Platform::Vulkan)
    set(GEOGL_GENERATOR_LIBS "${GEOGL_GENERATOR_LIBS} $<TARGET_OBJECTS:GEOGL::Platform::Vulkan>")
    set(GEOGL_PLATFORM_EXISTS TRUE)
    set(GEOGL_VULKAN_BUILD_FLAG 1)
    message("-- Building GEOGL::Engine with Vulkan support.")
endif(GEOGL_BUILD_WITH_VULKAN)

# make sure a platform is selected. If not, error
if (NOT(GEOGL_PLATFORM_EXISTS STREQUAL "TRUE"))
    message(FATAL_ERROR "No platform selected for GEOGL. Please select a platform.")
endif(NOT(GEOGL_PLATFORM_EXISTS STREQUAL "TRUE"))



#######################################################################
#           Add Dummy target relating to macro definitions            #
#######################################################################
add_library(GEOGL_Interface INTERFACE)
#target_compile_definitions("GEOGL_Interface" INTERFACE GEOGL_BUILD_LIBRARY)
target_compile_definitions("GEOGL_Interface" INTERFACE GEOGL_VERSION_MAJOR=${GEOGL_VERSION_MAJOR})
target_compile_definitions("GEOGL_Interface" INTERFACE GEOGL_VERSION_MINOR=${GEOGL_VERSION_MINOR})
target_compile_definitions("GEOGL_Interface" INTERFACE GEOGL_VERSION_PATCH=${GEOGL_VERSION_PATCH})
target_compile_definitions("GEOGL_Interface" INTERFACE GEOGL_VERSION_FULL="${GEOGL_VERSION_FULL}")
target_compile_definitions("GEOGL_Interface" INTERFACE GEOGL_ENGINE_NAME="GEOGL-Engine-${GEOGL_VERSION_FULL}")
target_compile_definitions("GEOGL_Interface" INTERFACE GEOGL_ENGINE_NAME_NO_VERSION="GEOGL Engine")
target_compile_definitions("GEOGL_Interface" INTERFACE GEOGL_BUILD_WITH_VULKAN=${GEOGL_VULKAN_BUILD_FLAG})
target_compile_definitions("GEOGL_Interface" INTERFACE GEOGL_BUILD_WITH_OPENGL=${GEOGL_OGL_BUILD_FLAG})
target_compile_definitions("GEOGL_Interface" INTERFACE GEOGL_ENABLE_IGPU=${GEOGL_ENABLE_IGPU_DEF})

target_include_directories("GEOGL_Interface" INTERFACE include
        Modules/Platform/OpenGL/include
        Modules/Platform/Vulkan/include
        Modules/Utils/include
        ../../Dependencies/Nlohmann/JSON/include
        ../../Dependencies/glfw-3.3.2/include
        ../../Dependencies/GLAD/include
        ../../Dependencies/spdlog-1.8.2/include
        ../../Dependencies/STB/include
        ../../Dependencies/GLM
        )

#target_precompile_headers(GEOGL_Interface INTERFACE Modules/Utils/Headers/PCH.hpp)

#######################################################################
#                      Build all Dependency DLLs                      #
#######################################################################

# Include Utils And Dependencies
add_subdirectory(Modules/Utils)

# Include Platforms
add_subdirectory(Modules/Platform)

# Process Dependencies
set(GEOGL_BUILD_DEPS GEOGL::Utils ${GEOGL_PLATFORM_LIBS})
set(GEOGL_LINK_DEPS GEOGL::Utils ${GEOGL_PLATFORM_LIBS})
if(NOT BUILD_SHARED_LIBS)
    set(GEOGL_GENERATOR_LIBS $<TARGET_OBJECTS:GEOGL::Utils> ${GEOGL_GENERATOR_LIBS})
endif()
message( -- ${GEOGL_GENERATOR_LIBS})

# Build the GEOGL_DLL
add_library(GEOGL ${GEOGL_LIBRARY_TYPE}
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
        )


set(GEOGL_LIBRARY_NAME GEOGL)

# Add Dependencies
add_dependencies(${GEOGL_LIBRARY_NAME} ${GEOGL_BUILD_DEPS})
target_link_libraries(${GEOGL_LIBRARY_NAME} ${GEOGL_LINK_DEPS} ${GEOGL_GENERATOR_LIBS} GEOGL_Interface)

target_include_directories("${GEOGL_LIBRARY_NAME}" PUBLIC include)

target_include_directories("${GEOGL_LIBRARY_NAME}" PRIVATE
        ../../Dependencies/GLAD/include # glad needs to stay here until imgui is on our render
        ../../Dependencies/imgui-docking/include
        )


target_precompile_headers("${GEOGL_LIBRARY_NAME}" PRIVATE Modules/Utils/Headers/PCH.hpp)

set_target_properties(${GEOGL_LIBRARY_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/
        )

set_target_properties(${GEOGL_LIBRARY_NAME} PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/
        )

add_library(GEOGL::Engine ALIAS ${GEOGL_LIBRARY_NAME})
