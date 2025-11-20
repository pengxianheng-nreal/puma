PROJECT_INIT_TARGET_VARIABLE(opencv_dshow)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fomit-frame-pointer -msse -msse2 -ffunction-sections -Wno-delete-non-virtual-dtor -Wno-unused-parameter")
set(DSHOW_DEPEND_LIBS framework::framework uuid ole32 oleaut32)
set(CAM_SRC cap_dshow.cpp)
add_library( ${TARGET_NAME} OBJECT ${CAM_SRC} )
target_include_directories(${TARGET_NAME} 
    PUBLIC 
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>")
    
target_link_libraries( ${TARGET_NAME} PUBLIC ${DSHOW_DEPEND_LIBS} )

set(OCV_DSHOW_LIB ${TARGET_NAME} PARENT_SCOPE)

PROJECT_INSTALL(${TARGET_NAME} ${TARGETS_EXPORT_NAME})