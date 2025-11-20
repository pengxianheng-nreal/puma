cmake_minimum_required( VERSION 3.13 )

include_directories(. libusb libusb/os)

list(APPEND LIB_USB_SRC
    libusb/hotplug.c
    libusb/os/linux_usbfs.c
    libusb/os/linux_netlink.c
    libusb/os/poll_posix.c
    libusb/os/threads_posix.c
    libusb/strerror.c
    libusb/io.c
    libusb/core.c
    libusb/descriptor.c
    libusb/sync.c
    )

if( BUILD_APPS OR FRAMEWORK_BUILD_APPS)
    add_library(libusb OBJECT ${LIB_USB_SRC})
else()
    add_library(libusb SHARED  ${LIB_USB_SRC})
    set_target_properties(libusb PROPERTIES OUTPUT_NAME "nr_libusb")
endif()

# if (ANDROID)
#     target_link_libraries(libusb log)
# endif()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -Wno-unused-parameter -Wno-unused-function -Wno-deprecated-declarations -Wno-format-zero-length -Wno-format -Wno-sign-compare")
