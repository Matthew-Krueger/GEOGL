####################################
#          GEOGL Platform          #
####################################

if(GEOGL_BUILD_WITH_OPENGL)
    add_subdirectory(OpenGL)
endif(GEOGL_BUILD_WITH_OPENGL)
if(GEOGL_BUILD_WITH_VULKAN)
    add_subdirectory(Vulkan)
endif(GEOGL_BUILD_WITH_VULKAN)