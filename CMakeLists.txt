cmake_minimum_required(VERSION 3.16)

project(GEOGL)

#if(NOT CMAKE_BUILD_TYPE)
#    set(CMAKE_BUILD_TYPE Release)
#endif()

###################################
#             Options             #
###################################
option(GEOGL_BUILD_WITH_OPENGL "Build GEOGL with OpenGL support" ON)
option(GEOGL_BUILD_WITH_VULKAN "Build GEOGL with Vulkan Support" OFF)


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
set(GEOGL_INCLUDE_DIRECTORIES
    Dependencies/Nlohmann/JSON/include
    Dependencies/GLFW/include
    Dependencies/GLAD/include
    Dependencies/spdlog/include
    Dependencies/STB/include Dependencies/GLM
)

# JSON
include_directories(Dependencies/Nlohmann/JSON/include)

# GLFW
add_subdirectory(Dependencies/GLFW)
include_directories(Dependencies/GLFW/include)

# GLAD
add_subdirectory(Dependencies/GLAD)
include_directories(Dependencies/GLAD/include)

# spdlog
add_subdirectory(Dependencies/spdlog)
include_directories(Dependencies/spdlog/include)
set_property(TARGET spdlog PROPERTY POSITION_INDEPENDENT_CODE ON)

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

###################################
#         Build Doxygen           #
###################################
set(DOXYGEN_GENERATE_HTML YES)
#set(DOXYGEN_CALLER_GRAPH YES)
#set(DOXYGEN_CALL_GRAPH YES)
set(DOXYGEN_EXTRACT_ALL YES)
set(DOXYGEN_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/Docs")
set(DOXYGEN_EXCLUDE_PATTERNS ",,*/Docs/*,*/cmake-build-debug/*,*/cmake-build-release/*,*/Dependencies/*,*/Build/*")
set(DOXYGEN_USE_MDFILE_AS_MAINPAGE README.md)
set(DOXYGEN_UML_LOOK YES)

find_package(Doxygen REQUIRED)
doxygen_add_docs(${PROJECT_NAME}_Doxygen "${CMAKE_SOURCE_DIR}")