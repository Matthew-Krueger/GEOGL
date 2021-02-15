set(MODULE_NAME ${PROJECT_NAME}_OpenGL)

add_library(${MODULE_NAME} STATIC IO/OpenGLWindow.cpp IO/OpenGLWindow.hpp imgui_impl_opengl3.h imgui_impl_opengl3.cpp IO/OpenGLInput.hpp IO/OpenGLInput.cpp IO/OpenGLKeyCodes.cpp IO/OpenGLKeyCodes.hpp)
set_property(TARGET ${MODULE_NAME} PROPERTY POSITION_INDEPENDENT_CODE ON)

target_precompile_headers("${MODULE_NAME}" PRIVATE ../../Utils/Headers/PCH.hpp)


add_library(GEOGL::Platform::OpenGL ALIAS ${MODULE_NAME})