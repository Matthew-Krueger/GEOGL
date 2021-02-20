set(MODULE_NAME ${PROJECT_NAME}_Example)

if(WIN32)
    #set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} /link /SUBSYSTEM:WINDOWS /entry:main)
    #message(${CMAKE_BUILD_TYPE})
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        message("-- Debug Mode For Win32")
    else(CMAKE_BUILD_TYPE STREQUAL "Debug")
        message("-- Release Mode for Win32")
        SET(OPTIONS WIN32)
    endif(CMAKE_BUILD_TYPE STREQUAL "Debug")

    set(ADDITIONAL_WIN32_FILES Resources/imageIcon.rc)

endif(WIN32)

add_executable(${MODULE_NAME} ${OPTIONS} SandboxApplication.cpp SandboxApplication.hpp ${ADDITIONAL_WIN32_FILES})

add_dependencies(${MODULE_NAME} GEOGL::Engine)
target_link_libraries(${MODULE_NAME} GEOGL::Engine)
#target_include_directories(${MODULE_NAME} PRIVATE ../GEOGL/include)

set_target_properties(${MODULE_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/
        )