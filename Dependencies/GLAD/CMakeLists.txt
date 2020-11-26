#############################################
# All Copyright is according to LICENSE.txt #
#############################################

cmake_minimum_required(VERSION 3.16.3)
project(GLAD C)

set(CMAKE_C_STANDARD 99)
include_directories(include)

add_library(GLAD STATIC src/glad.c)
set_property(TARGET GLAD PROPERTY POSITION_INDEPENDENT_CODE ON)
