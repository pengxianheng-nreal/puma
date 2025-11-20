PROJECT_INIT_TARGET_VARIABLE(generic_camera)

set(CAM_DEPEND_LIBS framework::framework Threads::Threads yuv)
set(CAM_INCLUDE_DIRS
"${TARGET_INCLUDE_PATH}"
"${PROJECT_BINARY_DIR}"
"${PROJECT_SOURCE_DIR}"
"${EXTERNAL_INCLUDE_DIR}"
)

set(CAM_SRC camera_factory.cpp)

if (WIN32)
    add_subdirectory(ocv_dshow)
    list(APPEND CAM_SRC camera_dshow.cpp)
    list(APPEND CAM_DEPEND_LIBS ${OCV_DSHOW_LIB})
    message(STATUS "CAM_DEPEND_LIBS = ${CAM_DEPEND_LIBS}")
    if (BUILD_SHARED_LIBS)
        list(APPEND CAM_DEPEND_LIBS -lz)
    endif()
else()
    if (APPLE)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-variable")
        add_compile_options(-x objective-c++)
        list(APPEND CAM_SRC camera_mac.mm)
        list(APPEND CAM_DEPEND_LIBS ${MAC_LIBUSB_LIBS} 
            "-framework IOKit" "-framework CoreFoundation" "-framework AppKit" "-framework CoreMedia" "-framework CoreVideo" "-framework AVFoundation"
            )
    else()
        list(APPEND CAM_SRC camera_v4l2.cpp camera_uvc.cpp)
        list(APPEND CAM_DEPEND_LIBS uvc libusb)
    endif()
endif()


SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-parameter")


message(STATUS "TARGET_NAME=${TARGET_NAME}")
add_library( ${TARGET_NAME} OBJECT ${CAM_SRC} )
target_link_libraries( ${TARGET_NAME} PUBLIC ${CAM_DEPEND_LIBS})
target_include_directories( ${TARGET_NAME} 
    PUBLIC 
    "$<BUILD_INTERFACE:${CAM_INCLUDE_DIRS}>" )

set(GENERIC_CAM_DEPEND_LIBS ${CAM_DEPEND_LIBS} ${TARGET_NAME} ${OCV_DSHOW_LIB} PARENT_SCOPE)

PROJECT_INSTALL(${TARGET_NAME} ${TARGETS_EXPORT_NAME})