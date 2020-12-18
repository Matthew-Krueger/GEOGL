set(MODULE_NAME ${PROJECT_NAME}_OpenGL)

add_library(${MODULE_NAME} STATIC OpenGLCompileTest.cpp OpenGLCompileTest.hpp)
set_property(TARGET ${MODULE_NAME} PROPERTY POSITION_INDEPENDENT_CODE ON)

target_precompile_headers("${MODULE_NAME}" PRIVATE ../../Utils/Headers/PCH.hpp)

add_library(GEOGL::Platform::OpenGL ALIAS ${MODULE_NAME})