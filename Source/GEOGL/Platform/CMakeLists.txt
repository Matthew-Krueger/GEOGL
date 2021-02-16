####################################
#          GEOGL Platform          #
####################################

if(GEOGL_BUILD_WITH_OPENGL)
    add_subdirectory(OpenGL)
endif(GEOGL_BUILD_WITH_OPENGL)
if(GEOGL_BUILD_WITH_VULKAN)
    #message(FATAL_ERROR "Vulkan is not yet supported")
    add_subdirectory(Vulkan)
endif(GEOGL_BUILD_WITH_VULKAN)