

######################################
#         Add Vulkan Library         #
######################################
add_library(GEOGL_Platform_Vulkan ${GEOGL_LIBRARY_TYPE}
        Rendering/VulkanGraphicsContext.cpp Rendering/VulkanGraphicsContext.hpp Rendering/VulkanRendererAPI.cpp Rendering/VulkanRendererAPI.hpp VulkanExtensionsAndCallabacks/VulkanCallbacks.cpp VulkanExtensionsAndCallabacks/VulkanCallbacks.hpp VulkanExtensionsAndCallabacks/VulkanExtensions.cpp VulkanExtensionsAndCallabacks/VulkanExtensions.hpp)

######################################
#     Set name for use elsewhere     #
######################################
set(GEOGL_LIBRARY_NAME GEOGL_Platform_Vulkan)

######################################
#         Linker Dependencies        #
######################################
add_dependencies("${GEOGL_LIBRARY_NAME}" GEOGL::Utils glfw )
target_link_libraries("${GEOGL_LIBRARY_NAME}" GEOGL::Utils glfw ${VULKAN_LIB_LIST})

######################################
#      PreProcessor Definitions      #
######################################
target_compile_definitions("${GEOGL_LIBRARY_NAME}" PRIVATE GEOGL_BUILD_LIBRARY)

######################################
#         Precompiled Headers        #
######################################
target_precompile_headers("${GEOGL_LIBRARY_NAME}" PRIVATE ../../Utils/Headers/PCH.hpp)

######################################
#         Emit to bin Folder         #
######################################
set_target_properties("${GEOGL_LIBRARY_NAME}" PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/
        )

set_target_properties("${GEOGL_LIBRARY_NAME}" PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/
        )

######################################
#     Add Alias target for looks     #
######################################
add_library(GEOGL::Platform::Vulkan ALIAS ${GEOGL_LIBRARY_NAME})