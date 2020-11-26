cmake_minimum_required(VERSION 3.16.3)
project(STB_LIB C)

set(CMAKE_C_STANDARD 99)
include_directories(include)

add_library(STB_LIB STATIC include/STB/stb_image.h stb_image.c)
set_property(TARGET STB_LIB PROPERTY POSITION_INDEPENDENT_CODE ON)