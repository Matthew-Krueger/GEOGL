######################################
#         Add Vulkan Library         #
######################################
add_library(GEOGL_Platform_OpenGL SHARED

        IO/Window.cpp
        IO/Window.hpp
        IO/Input.hpp
        IO/Input.cpp
        IO/KeyCodes.cpp
        IO/KeyCodes.hpp

        imgui_impl_opengl3.h
        imgui_impl_opengl3.cpp

        )
######################################
#     Set name for use elsewhere     #
######################################
set(GEOGL_LIBRARY_NAME GEOGL_Platform_OpenGL)

######################################
#         Linker Dependencies        #
######################################
add_dependencies("${GEOGL_LIBRARY_NAME}" GEOGL::Utils glfw)
target_link_libraries("${GEOGL_LIBRARY_NAME}" GEOGL::Utils glfw)

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
target_compile_definitions("${GEOGL_LIBRARY_NAME}" PUBLIC GEOGL_VERSION_MAJOR=${GEOGL_VERSION_MAJOR})
target_compile_definitions("${GEOGL_LIBRARY_NAME}" PUBLIC GEOGL_VERSION_MINOR=${GEOGL_VERSION_MINOR})
target_compile_definitions("${GEOGL_LIBRARY_NAME}" PUBLIC GEOGL_VERSION_PATCH=${GEOGL_VERSION_PATCH})
target_compile_definitions("${GEOGL_LIBRARY_NAME}" PUBLIC GEOGL_VERSION_FULL="${GEOGL_VERSION_FULL}")
target_compile_definitions("${GEOGL_LIBRARY_NAME}" PUBLIC GEOGL_ENGINE_NAME="GEOGL-Engine-${GEOGL_VERSION_FULL}")
target_compile_definitions("${GEOGL_LIBRARY_NAME}" PUBLIC GEOGL_ENGINE_NAME_NO_VERSION="GEOGL Engine")
target_compile_definitions("${GEOGL_LIBRARY_NAME}" PUBLIC GEOGL_BUILD_WITH_VULKAN=${GEOGL_VULKAN_BUILD_FLAG})
target_compile_definitions("${GEOGL_LIBRARY_NAME}" PUBLIC GEOGL_BUILD_WITH_OPENGL=${GEOGL_OGL_BUILD_FLAG})
target_compile_definitions("${GEOGL_LIBRARY_NAME}" PUBLIC GEOGL_ENABLE_IGPU=${GEOGL_ENABLE_IGPU_DEF})

######################################
#         Precompiled Headers        #
######################################
target_precompile_headers("${GEOGL_LIBRARY_NAME}" PRIVATE ../../Utils/Headers/PCH.hpp)

######################################
#     Add to Include Directories     #
######################################
target_include_directories("${GEOGL_LIBRARY_NAME}" PUBLIC include ${GEOGL_INCLUCE_DIRECTORIES})

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