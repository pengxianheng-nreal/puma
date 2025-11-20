project(external C CXX)
message(STATUS "External CMAKE_BUILD_TYPE= ${CMAKE_BUILD_TYPE}")
set(BUILD_SHARED_LIBS OFF)

set(BUILD_TESTING OFF)


MESSAGE(STATUS "operation system is ${CMAKE_SYSTEM}")
 
IF (CMAKE_SYSTEM_NAME MATCHES "Linux")
	MESSAGE(STATUS "current platform: Linux ")
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Windows")
	MESSAGE(STATUS "current platform: Windows")
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "FreeBSD")
	MESSAGE(STATUS "current platform: FreeBSD")
ELSE ()
	MESSAGE(STATUS "other platform: ${CMAKE_SYSTEM_NAME}")
ENDIF (CMAKE_SYSTEM_NAME MATCHES "Linux")
 

if (NOT LINUX)
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-tautological-pointer-compare")
endif()


if (ANDROID)
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wno-unused-parameter")
endif()

add_subdirectory(libyuv)
#Use libuvc for uvc camera stream
if (NOT WIN32 AND NOT APPLE)
add_subdirectory(libuvc)
PROJECT_INSTALL(uvc ${TARGETS_EXPORT_NAME})
endif()
#Use libusb for uvc usb control flow 
if (NOT WIN32 AND NOT APPLE)
add_subdirectory(libusb)
PROJECT_INSTALL(libusb ${TARGETS_EXPORT_NAME})
target_include_directories(uvc PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/libusb/libusb ${CMAKE_CURRENT_SOURCE_DIR}/libyuv/include)
endif()

# encryption libs
#add_subdirectory(libcrypt)

#PROJECT_INSTALL(crypt_obj ${TARGETS_EXPORT_NAME})

message(STATUS "TARGETS_EXPORT_NAME= ${TARGETS_EXPORT_NAME}")
PROJECT_INSTALL(yuv ${TARGETS_EXPORT_NAME})
