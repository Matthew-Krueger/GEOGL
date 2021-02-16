set(MODULE_NAME ${PROJECT_NAME}_Vulkan)

add_library(${MODULE_NAME} STATIC IO/VulkanInput.cpp IO/VulkanInput.hpp IO/VulkanKeyCodes.cpp IO/VulkanKeyCodes.hpp IO/VulkanWindow.cpp IO/VulkanWindow.hpp VulkanExtensions/VulkanExtensions.cpp VulkanExtensions/VulkanExtensions.hpp)
set_property(TARGET ${MODULE_NAME} PROPERTY POSITION_INDEPENDENT_CODE ON)

target_precompile_headers("${MODULE_NAME}" PRIVATE ../../Utils/Headers/PCH.hpp)

target_compile_definitions("${MODULE_NAME}" PRIVATE GEOGL_BUILD_LIBRARY)
target_compile_definitions("${MODULE_NAME}" PRIVATE GEOGL_VERSION_MAJOR=${GEOGL_VERSION_MAJOR})
target_compile_definitions("${MODULE_NAME}" PRIVATE GEOGL_VERSION_MINOR=${GEOGL_VERSION_MINOR})
target_compile_definitions("${MODULE_NAME}" PRIVATE GEOGL_VERSION_PATCH=${GEOGL_VERSION_PATCH})
target_compile_definitions("${MODULE_NAME}" PRIVATE GEOGL_VERSION_FULL="${GEOGL_VERSION_FULL}")
target_compile_definitions("${MODULE_NAME}" PRIVATE GEOGL_ENGINE_NAME="GEOGL-Engine-${GEOGL_VERSION_FULL}")
target_compile_definitions("${MODULE_NAME}" PRIVATE GEOGL_ENGINE_NAME_NO_VERSION="GEOGL Engine")
target_compile_definitions("${MODULE_NAME}" PRIVATE GEOGL_BUILD_WITH_VULKAN=${GEOGL_VULKAN_BUILD_FLAG})
target_compile_definitions("${MODULE_NAME}" PRIVATE GEOGL_BUILD_WITH_OPENGL=${GEOGL_OGL_BUILD_FLAG})

add_custom_command(
        TARGET ${MODULE_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/${PATH_TO_GEOGL}/Source/GEOGL/Platform/Vulkan/vk_layer_settings.txt" "${CMAKE_BINARY_DIR}/bin/vk_layer_settings.txt"
        COMMENT "Copying vk_layer_settings.txt to build directory"
)
add_library(GEOGL::Platform::Vulkan ALIAS ${MODULE_NAME})