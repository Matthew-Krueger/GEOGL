cmake_minimum_required(VERSION 3.16)

project(GEOGL)

#if(NOT CMAKE_BUILD_TYPE)
#    set(CMAKE_BUILD_TYPE Release)
#endif()

###################################
#             Options             #
###################################
option(GEOGL_BUILD_SHARED_LIBS              "Build shared libraries of DLL and Dependencies"                ON)
    # The build shared libs option will link GEOGL and dependencies into one large DLL
    # I don't feel like dealing with MSVC's annoying stuff getting linking across that many dlls to work.
    # maybe in the future, should the need arise by a large codebase
option(GEOGL_BUILD_WITH_OPENGL              "Build GEOGL with OpenGL support"                               ON)
option(GEOGL_BUILD_WITH_VULKAN              "Build GEOGL with Vulkan Support"                               OFF)
option(GEOGL_ENABLE_IGPU                    "Build GEOGL to enable IGPU support in Vulkan"                  ON)
option(GEOGL_TRACK_MEMORY_ALLOC             "Build GEOGL to track memory allocations (slow)"                ON)
option(GEOGL_BUILD_WITH_PROFILING           "Build GEOGL to self-profile"                                   OFF)
option(GEOGL_BUILD_WITH_RENDERER_PROFILING  "Build GEOGL to self-profile the Renderer"                      OFF)
option(GEOGL_CACHE_COMPILED_SHADERS         "Maintain a cache of compiled shaders"                          ON)
option(GEOGL_BUILD_WITH_SSE2                "Build GEOGL with SSE2"                                         ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)



###################################
#         Testing Options         #
###################################
option(GEOGL_TEST_SHARED_PTR                "Test the GEOGL::shared_ptr class"                              ON)

###################################
#              Info               #
###################################

message(STATUS "GEOGL Build type: ${CMAKE_BUILD_TYPE}")

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

set(GEOGL_VERSION_MAJOR "0")
set(GEOGL_VERSION_MINOR "0")
set(GEOGL_VERSION_PATCH "1")
set(GEOGL_VERSION_TWEAK "2")
set(GEOGL_VERSION "${GEOGL_VERSION_MAJOR}.${GEOGL_VERSION_MINOR}")
set(GEOGL_VERSION_FULL "${GEOGL_VERSION}.${GEOGL_VERSION_PATCH}-${GEOGL_VERSION_TWEAK}")



###################################
#           Dependencies          #
###################################

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/CMake)

add_library(sse2 INTERFACE IMPORTED)
if(MSVC)
    #set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
    target_compile_options(sse2 INTERFACE /arch:sse2)
else()
    target_compile_options(sse2 INTERFACE -msse2)
endif()

set(CMAKE_CXX_VISIBILITY_PRESET hidden)

# Catch2
include_directories(Dependencies/Catch2/include)

# JSON
include_directories(Dependencies/Nlohmann/JSON/include)

# GLFW
add_subdirectory(Dependencies/glfw-3.3.4)

# GLAD
add_subdirectory(Dependencies/GLAD)

# spdlog
add_subdirectory(Dependencies/spdlog-1.8.5)
set_property(TARGET spdlog PROPERTY POSITION_INDEPENDENT_CODE ON)

# stbimage
add_subdirectory(Dependencies/STB)

# OpenMP
find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    message("-- Using OpenMP for Multiprocessing Acceleration.")
endif()

# ImGui
add_subdirectory(Dependencies/imgui-docking)

###################################
#          Build GEOGL            #
###################################
include_directories(Engine/Source/GEOGL/include)
add_subdirectory(Engine/Source)
add_subdirectory(Engine/Tests)

###################################
#          Build Examples         #
###################################
add_subdirectory(Projects/Example2DGame)

###################################
#          Build Sandbox          #
###################################
add_subdirectory(Projects/Sandbox)

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