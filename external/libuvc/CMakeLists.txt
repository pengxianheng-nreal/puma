cmake_minimum_required( VERSION 3.13 )

project(libuvc)

include_directories(include ${LIBUSB_INCLUDE_DIRS} ${EXTERNAL_INCLUDE_DIR})

SET(SOURCES src/ctrl.c src/ctrl-gen.c src/device.c src/diag.c
           src/frame.c src/init.c src/stream.c
           src/misc.c)

add_library(uvc OBJECT ${SOURCES})

target_link_libraries(uvc
    PUBLIC
    -pthread
    libusb)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -Wno-unused-but-set-variable -Wno-unused-parameter -Wno-implicit-function-declaration -Wno-unused-function -Wno-deprecated-declarations -Wno-format-zero-length -Wno-format -Wno-unused-variable")

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-implicit-fallthrough -Wno-misleading-indentation")
endif()

