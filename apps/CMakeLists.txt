add_library( app_base INTERFACE )

add_compile_definitions(NRPLUGIN)

find_package(OpenCV REQUIRED)

if(WIN32)
    set (OPENCV_LIBS opencv::opencv)
else ()
    set (OPENCV_LIBS opencv::opencv_imgproc_alias opencv::opencv_highgui_alias)
endif()

target_include_directories( app_base
    INTERFACE
    
    )
target_link_libraries( app_base
    INTERFACE
    puma_static
    interface
    )

if (ANDROID)
target_link_libraries(app_base INTERFACE dl)
elseif(WIN32)
target_link_libraries(app_base INTERFACE 
   -lws2_32
   ${OPENCV_LIBS}
   )
else ()
target_link_libraries(app_base INTERFACE 
    dl
    ${OPENCV_LIBS}
   )
endif() # end if ANDROID


SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-parameter -Wno-unused-variable")

APP_TARGET(app_camera app_camera.cc app_base)


