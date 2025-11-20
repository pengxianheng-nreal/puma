cmake_minimum_required(VERSION 3.13)

FILE ( GLOB_RECURSE	YUV_SRCS source/*.cc )
LIST ( SORT			YUV_SRCS )

IF(ANDROID)
IF (CMAKE_CL_64)
add_compile_definitions(__ARM_NEON__)
ENDIF()
ENDIF()

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-parameter")
INCLUDE_DIRECTORIES(include)

add_library(yuv OBJECT ${YUV_SRCS})