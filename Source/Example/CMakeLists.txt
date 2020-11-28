set(MODULE_NAME ${PROJECT_NAME}_Example)

add_executable(${MODULE_NAME} main.cpp)

if(WIN32)
    set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} /link /subsystem:windows /entry:mainCRTStartup)
endif(WIN32)

target_link_libraries(${MODULE_NAME} ${PROJECT_NAME})
#target_include_directories(${MODULE_NAME} PRIVATE ../GEOGL/include)

set_target_properties(${MODULE_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/
        )