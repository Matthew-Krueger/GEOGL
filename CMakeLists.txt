cmake_minimum_required(VERSION 3.16)

project(GEOGL)

###################################
#              Info               #
###################################


set(GEOGL_VERSION_MAJOR "0")
set(GEOGL_VERSION_MINOR "0")
set(GEOGL_VERSION_PATCH "1")
set(GEOGL_VERSION_TWEAK "1")
set(GEOGL_VERSION "${GEOGL_VERSION_MAJOR}.${GEOGL_VERSION_MINOR}")
set(GEOGL_VERSION_FULL "${GEOGL_VERSION}.${GEOGL_VERSION_PATCH}-${GEOGL_VERSION_TWEAK}")



###################################
#           Dependencies          #
###################################

# GLFW
add_subdirectory(Dependencies/GLFW)
include_directories(Dependencies/GLFW/include)

# GLAD
add_subdirectory(Dependencies/GLAD)
include_directories(Dependencies/GLAD/include)

# spdlog
add_subdirectory(Dependencies/spdlog)
include_directories(Dependencies/spdlog/include)

# stbimage
add_subdirectory(Dependencies/STB)
include_directories(Dependencies/STB/include)

# GLM
include_directories(Dependencies/GLM)
###################################
#          Build GEOGL            #
###################################
include_directories(Source/GEOGL/include)
add_subdirectory(Source/GEOGL)

###################################
#          Build Example          #
###################################
add_subdirectory(Source/Example)