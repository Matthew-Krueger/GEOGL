set(MODULE_NAME ${PROJECT_NAME}_Vulkan)

add_library(${MODULE_NAME} STATIC IO/VulkanInput.cpp IO/VulkanInput.hpp IO/VulkanKeyCodes.cpp IO/VulkanKeyCodes.hpp IO/VulkanWindow.cpp IO/VulkanWindow.hpp)
set_property(TARGET ${MODULE_NAME} PROPERTY POSITION_INDEPENDENT_CODE ON)

target_precompile_headers("${MODULE_NAME}" PRIVATE ../../Utils/Headers/PCH.hpp)


add_library(GEOGL::Platform::Vulkan ALIAS ${MODULE_NAME})