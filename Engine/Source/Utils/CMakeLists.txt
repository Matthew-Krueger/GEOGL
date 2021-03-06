######################################
#    Track memory Allocations API    #
######################################
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Compiling GEOGL::Utils in debug mode. Determining what additional to add")
    if(GEOGL_TRACK_MEMORY_ALLOC)
        message("-- Enabling Memory Allocation")
    endif()
else()
    message(STATUS "Compiling GEOGL::Utils as release config. No additional files will be added.")
endif()

######################################
#         Add Utils Library          #
######################################
add_library(GEOGL_Utils ${GEOGL_LIBRARY_TYPE}

        ${TRACK_MEMORY_ALLOC_CPP}
        Callbacks.cpp
        Callbacks.hpp
        Headers/Dependencies.hpp
        InputCodes.hpp
        InputCodesConverter.cpp
        InputCodesConverter.hpp
        Settings.cpp
        Settings.hpp
        include/GEOGL/API_Utils/DLLExportsAndTraps.hpp

        Logging/PublicLog.hpp
        Logging/PrivateLog.hpp
        Logging/LoggingFunctions.cpp

        include/GEOGL/Utils.hpp
        include/GEOGL/Dependencies.hpp

        Headers/PCH.cpp
        Headers/PCH.hpp
        TimeStep.hpp
        Memory/TrackMemoryAllocations.hpp
        Memory/TrackMemoryAllocations.cpp

        Timing/Timer.cpp Timing/Timer.hpp

        Headers/Refs.hpp Memory/Pointers.hpp)

######################################
#     Set name for use elsewhere     #
######################################
set(GEOGL_LIBRARY_NAME GEOGL_Utils)

######################################
#         Linker Dependencies        #
######################################
# STB_LIB spdlog ImGui GEOGL_Interface
set_target_properties(${GEOGL_LIBRARY_NAME} PROPERTIES POSITION_INDEPENDENT_CODE on)
add_dependencies(${GEOGL_LIBRARY_NAME} STB_LIB spdlog ImGui GEOGL_Interface)
target_link_libraries(${GEOGL_LIBRARY_NAME} STB_LIB spdlog ImGui GEOGL_Interface)

######################################
#      PreProcessor Definitions      #
######################################
target_compile_definitions("${GEOGL_LIBRARY_NAME}" PRIVATE GEOGL_BUILD_LIBRARY)

######################################
#         Precompiled Headers        #
######################################
target_precompile_headers("${GEOGL_LIBRARY_NAME}" PRIVATE Headers/PCH.hpp)

######################################
#     Add to Include Directories     #
######################################

#target_include_directories(${GEOGL_LIBRARY_NAME} PRIVATE
#        ../../../../Dependencies/glfw-3.3.2/include
#        ../../../../Dependencies/GLAD/include
#        ../../../../Dependencies/imgui-docking/include
#        )

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
add_library(GEOGL::Utils ALIAS "${GEOGL_LIBRARY_NAME}")
