#######################################################################
#       Configure Optional Dependencies Depending on Build Flags      #
#######################################################################
# Optional Platform Libs
set(GEOGL_OPENGL_BUILD_FLAG 0)
set(GEOGL_VULKAN_BUILD_FLAG 0)
set(GEOGL_PLATFORM_DEPS "")
set(GEOGL_PLATFORM_DEPS_WINDOWING "")

if(BUILD_SHARED_LIBS)
    set(GEOGL_LIBRARY_TYPE SHARED)
    set(GEOGL_MAIN_LIBRARY_TYPE SHARED)
    message("-- Building GEOGL and dependencies as DLLs")
else(BUILD_SHARED_LIBS)
    set(GEOGL_LIBRARY_TYPE OBJECT)
    set(GEOGL_MAIN_LIBRARY_TYPE STATIC)
    message("-- Building GEOGL and dependencies as Object Libraries")
endif(BUILD_SHARED_LIBS)

#######################################################################
#                Configure IGPU Support (Where applicable)            #
#######################################################################
if(GEOGL_ENABLE_IGPU)
    set(GEOGL_ENABLE_IGPU_DEF 1)
else()
    set(GEOGL_ENABLE_IGPU_DEF 0)
endif()

#######################################################################
#                           SELECT PLATFORM                           #
#######################################################################

if(GEOGL_BUILD_WITH_OPENGL OR GEOGL_BUILD_WITH_VULKAN)
    set(GEOGL_GLFW_BUILD_FLAG 1)
    set(GEOGL_PLATFORM_DPES_WINDOWING ${GEOGL_PLATFORM_DEPS_WINDOWING} GEOGL::Platform::GLFW)
endif()

if(GEOGL_BUILD_WITH_OPENGL)
    message("-- Building GEOGL with OpenGL Support")
    set(GEOGL_PLATFORM_EXISTS TRUE)
    set(GEOGL_OPENGL_BUILD_FLAG 1)
    set(GEOGL_PLATFORM_DEPS ${GEOGL_PLATFORM_DEPS} GEOGL::Platform::OpenGL)

endif(GEOGL_BUILD_WITH_OPENGL)
if(GEOGL_BUILD_WITH_VULKAN)

    message("-- Building GEOGL with Vulkan Support")
    set(GEOGL_PLATFORM_EXISTS TRUE)
    set(GEOGL_VULKAN_BUILD_FLAG 1)
    set(GEOGL_PLATFORM_DEPS ${GEOGL_PLATFORM_DEPS} GEOGL::Platform::Vulkan)

endif(GEOGL_BUILD_WITH_VULKAN)



# make sure a platform is selected. If not, error
if (NOT(GEOGL_PLATFORM_EXISTS STREQUAL "TRUE"))
    #message(FATAL_ERROR "No platform selected for GEOGL. Please select a platform.")
endif(NOT(GEOGL_PLATFORM_EXISTS STREQUAL "TRUE"))

###################################
#      Find Vulkan (If Enabled)   #
###################################
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

#######################################################################
#           Add Dummy target relating to macro definitions            #
#######################################################################
add_library(GEOGL_Interface INTERFACE)
target_compile_definitions("GEOGL_Interface" INTERFACE GEOGL_VERSION_MAJOR=${GEOGL_VERSION_MAJOR})
target_compile_definitions("GEOGL_Interface" INTERFACE GEOGL_VERSION_MINOR=${GEOGL_VERSION_MINOR})
target_compile_definitions("GEOGL_Interface" INTERFACE GEOGL_VERSION_PATCH=${GEOGL_VERSION_PATCH})
target_compile_definitions("GEOGL_Interface" INTERFACE GEOGL_VERSION_FULL="${GEOGL_VERSION_FULL}")
target_compile_definitions("GEOGL_Interface" INTERFACE GEOGL_ENGINE_NAME="GEOGL-Engine-${GEOGL_VERSION_FULL}")
target_compile_definitions("GEOGL_Interface" INTERFACE GEOGL_ENGINE_NAME_NO_VERSION="GEOGL Engine")
target_compile_definitions("GEOGL_Interface" INTERFACE GEOGL_BUILD_WITH_VULKAN=${GEOGL_VULKAN_BUILD_FLAG})
target_compile_definitions("GEOGL_Interface" INTERFACE GEOGL_BUILD_WITH_OPENGL=${GEOGL_OPENGL_BUILD_FLAG})
target_compile_definitions("GEOGL_Interface" INTERFACE GEOGL_ENABLE_IGPU=${GEOGL_ENABLE_IGPU_DEF})
target_compile_definitions("GEOGL_Interface" INTERFACE GEOGL_BUILD_WITH_GLFW=${GEOGL_GLFW_BUILD_FLAG})
if(NOT BUILD_SHARED_LIBS)
    target_compile_definitions("GEOGL_Interface" INTERFACE GEOGL_BUILD_STATIC)
endif(NOT BUILD_SHARED_LIBS)

target_include_directories("GEOGL_Interface" INTERFACE include
        Platform/OpenGL/include
        Platform/Vulkan/include
        Platform/GLFW/include
        Utils/include
        ../../Dependencies/Nlohmann/JSON/include
        ../../Dependencies/glfw-3.3.4/include
        ../../Dependencies/GLAD/include
        ../../Dependencies/spdlog-1.8.5/include
        ../../Dependencies/STB/include
        ../../Dependencies/GLM-0.9.9.8.7/glm
        ../../Dependencies/imgui-docking/include
        )

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    if(GEOGL_TRACK_MEMORY_ALLOC)
        target_compile_definitions("GEOGL_Interface" INTERFACE GEOGL_TRACK_MEMORY_ALLOC_FLAG=1)
    endif()
endif()

#target_precompile_headers(GEOGL_Interface INTERFACE Modules/Utils/Headers/PCH.hpp)

#######################################################################
#                      Build all Dependency DLLs                      #
#######################################################################

# Include Utils And Dependencies
add_subdirectory(Utils)

# Include Platforms
add_subdirectory(Platform)

add_subdirectory(GEOGL)