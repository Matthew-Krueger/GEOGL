####################################
#          GEOGL Platform          #
####################################


if(GEOGL_BUILD_WITH_OPENGL OR GEOGL_BUILD_WITH_VULKAN)
    add_subdirectory(GLFW)
endif()

if(GEOGL_BUILD_WITH_OPENGL)
    #find OpenGL
    find_library(OpenGL REQUIRED)
    add_subdirectory(OpenGL)
endif(GEOGL_BUILD_WITH_OPENGL)
if(GEOGL_BUILD_WITH_VULKAN)
    #message(FATAL_ERROR "Vulkan is not yet supported")
    add_subdirectory(Vulkan)
endif(GEOGL_BUILD_WITH_VULKAN)