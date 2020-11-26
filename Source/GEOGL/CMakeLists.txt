set(MODULE_NAME ${PROJECT_NAME})

add_library(${MODULE_NAME} SHARED Utils/PCH.cpp Utils/PCH.hpp Logging/PrivateLog.hpp Logging/PublicLog.hpp Logging/LoggingFunctions.cpp Win32Exports.hpp include/GEOGL/GEOGL.hpp)

add_dependencies(${MODULE_NAME} glfw STB_LIB spdlog GLAD EASTL)
target_link_libraries(${MODULE_NAME} glfw STB_LIB ${VULKAN_LIB_LIST} spdlog GLAD EASTL)

if(WIN32)

    add_dependencies(${MODULE_NAME} Msvcrt)

endif(WIN32)

target_compile_definitions("${MODULE_NAME}" PRIVATE GEOGL_BUILD_LIBRARY)
target_compile_definitions("${MODULE_NAME}" PRIVATE GEOGL_VERSION_MAJOR=${GEOGL_VERSION_MAJOR})
target_compile_definitions("${MODULE_NAME}" PRIVATE GEOGL_VERSION_MINOR=${GEOGL_VERSION_MINOR})
target_compile_definitions("${MODULE_NAME}" PRIVATE GEOGL_VERSION_PATCH=${GEOGL_VERSION_PATCH})
target_compile_definitions("${MODULE_NAME}" PRIVATE GEOGL_ENGINE_NAME="${PROJECT_NAME}-${GEOGL_VERSION_FULL}")

target_precompile_headers("${MODULE_NAME}" PRIVATE Utils/PCH.hpp)

set_target_properties(${MODULE_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/
        )