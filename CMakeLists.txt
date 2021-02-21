cmake_minimum_required(VERSION 3.16)

project(GEOGL)

#if(NOT CMAKE_BUILD_TYPE)
#    set(CMAKE_BUILD_TYPE Release)
#endif()

###################################
#             Options             #
###################################
option(BUILD_SHARED_LIBS       "Build shared libraries of DLL and Dependencies"     ON)
option(GEOGL_BUILD_WITH_OPENGL "Build GEOGL with OpenGL support"                    ON)
option(GEOGL_BUILD_WITH_VULKAN "Build GEOGL with Vulkan Support"                    OFF)
option(GEOGL_ENABLE_IGPU       "Build GEOGL to enable IGPU support in Vulkan"       ON)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

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

set(CMAKE_CXX_VISIBILITY_PRESET hidden)

# JSON
include_directories(Dependencies/Nlohmann/JSON/include)

# GLFW
add_subdirectory(Dependencies/glfw-3.3.2)

# GLAD
add_subdirectory(Dependencies/GLAD)

# spdlog
add_subdirectory(Dependencies/spdlog-1.8.2)
set_property(TARGET spdlog PROPERTY POSITION_INDEPENDENT_CODE ON)

# stbimage
add_subdirectory(Dependencies/STB)

# GLM

# ImGui
add_subdirectory(Dependencies/imgui-docking)

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