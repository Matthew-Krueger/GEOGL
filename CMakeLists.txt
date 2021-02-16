cmake_minimum_required(VERSION 3.16)

project(GEOGL)

#if(NOT CMAKE_BUILD_TYPE)
#    set(CMAKE_BUILD_TYPE Release)
#endif()

###################################
#             Options             #
###################################
option(GEOGL_BUILD_WITH_OPENGL "Build GEOGL with OpenGL support"                ON)
option(GEOGL_BUILD_WITH_VULKAN "Build GEOGL with Vulkan Support"                ON)
option(GEOGL_ENABLE_IGPU       "Build GEOGL to enable IGPU support in Vulkan"   ON)


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

# ImGui
add_subdirectory(Dependencies/ImGui-1.81)
include_directories(Dependencies/ImGui-1.81/include)

###################################
#      Find Vulkan (If Enabled)   #
###################################
if(GEOGL_BUILD_WITH_VULKAN)
    # AUTO_LOCATE_VULKAN - accepted value ON or OFF
    # ON  - Use CMake to auto locate the Vulkan SDK.
    # OFF - Vulkan SDK path can be specified manually. This is helpful to test the build on various Vulkan version.
    option(AUTO_LOCATE_VULKAN "AUTO_LOCATE_VULKAN" ON)

    if(AUTO_LOCATE_VULKAN)
        message(STATUS "Attempting auto locate Vulkan using CMake......")

        # Find Vulkan Path using CMake's Vulkan Module
        # This will return Boolean 'Vulkan_FOUND' indicating the status of find as success(ON) or fail(OFF).
        # Include directory path - 'Vulkan_INCLUDE_DIRS' and 'Vulkan_LIBRARY' with required libraries.
        find_package(Vulkan)

        # Try extracting VulkanSDK path from ${Vulkan_INCLUDE_DIRS}
        if (NOT ${Vulkan_INCLUDE_DIRS} STREQUAL "")
            set(VULKAN_PATH ${Vulkan_INCLUDE_DIRS})
            STRING(REGEX REPLACE "/Include" "" VULKAN_PATH ${VULKAN_PATH})
        endif()

        if(NOT Vulkan_FOUND)
            # CMake may fail to locate the libraries but could be able to
            # provide some path in Vulkan SDK include directory variable
            # 'Vulkan_INCLUDE_DIRS', try to extract path from this.
            message(STATUS "Failed to locate Vulkan SDK, retrying again...")
            if(EXISTS "${VULKAN_PATH}")
                message(STATUS "Successfully located the Vulkan SDK: ${VULKAN_PATH}")
            else()
                message("Error: Unable to locate Vulkan SDK. Please turn off auto locate option by specifying 'AUTO_LOCATE_VULKAN' as 'OFF'")
                message("and specify manually path using 'VULKAN_SDK' and 'VULKAN_VERSION' variables in the CMakeLists.txt.")
                return()
            endif()
        endif()
    else()
        message(STATUS "Attempting to locate Vulkan SDK using manual path......")
        set(VULKAN_SDK "C:/VulkanSDK")
        set(VULKAN_VERSION "1.0.33.0")
        set(VULKAN_PATH "${VULKAN_SDK}/${VULKAN_VERSION}")
        message(STATUS "Using manual specified path: ${VULKAN_PATH}")

        # Check if manual set path exists
        if(NOT EXISTS "${VULKAN_PATH}")
            message("Error: Unable to locate this Vulkan SDK path VULKAN_PATH: ${VULKAN_PATH}, please specify correct path.
		For more information on correct installation process, please refer to subsection 'Getting started with Lunar-G SDK'
		and 'Setting up first project with CMake' in Chapter 3, 'Shaking hands with the device' in this book 'Learning Vulkan', ISBN - 9781786469809.")
            return()
        endif()
    endif()

    # Add any required preprocessor definitions here
    #add_definitions(-DVK_USE_PLATFORM_WIN32_KHR)

    # vulkan-1 library for build Vulkan application.
    if(EXISTS ${Vulkan_LIBRARY})
        set(VULKAN_LIB_LIST ${Vulkan_LIBRARY})
    else()
        set(VULKAN_LIB_LIST "vulkan-1")
    endif()

    if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
        # Include Vulkan header files from Vulkan SDK
        include_directories(AFTER ${VULKAN_PATH}/Include)

        # Link directory for vulkan-1
        link_directories(${VULKAN_PATH}/Bin;${VULKAN_PATH}/Lib;)
    endif()
endif(GEOGL_BUILD_WITH_VULKAN)


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