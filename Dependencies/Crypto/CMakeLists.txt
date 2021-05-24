#############################################
# All Copyright is according to LICENSE     #
#############################################

cmake_minimum_required(VERSION 3.16.3)
project(Crypto CXX)

set(CMAKE_CXX_STANDARD 17)
include_directories(include)

add_library(GEOGL_Crypto STATIC crc32.cpp keccak.cpp md5.cpp sha1.cpp sha3.cpp sha256.cpp include/Crypto/crc32.h include/Crypto/hmac.h include/Crypto/keccak.h include/Crypto/md5.h include/Crypto/sha1.h include/Crypto/sha3.h include/Crypto/sha256.h)
set_property(TARGET GEOGL_Crypto PROPERTY POSITION_INDEPENDENT_CODE ON)

add_library(GEOGL::Crypto ALIAS GEOGL_Crypto)