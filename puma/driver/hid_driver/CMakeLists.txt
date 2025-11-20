PROJECT_INIT_TARGET_VARIABLE(generic_hid)

set(HID_DEPEND_LIBS framework::framework Threads::Threads util)
set(INCLUDE_DIRS
"${TARGET_INCLUDE_PATH}"
"${PROJECT_BINARY_DIR}"
"${PROJECT_SOURCE_DIR}"
"${EXTERNAL_INCLUDE_DIR}"
${HID_INCLUDE_DIR}
)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-unused-variable -Wno-unused-parameter")

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-clobbered")
endif()

set(GEN_HID_SRC hidapi_driver.cpp hid_factory.cpp ${HID_SRC})

if(WIN32)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-cast-function-type")
endif()

if (NOT WIN32)
    if(ANDROID)
        list(APPEND HID_DEPEND_LIBS ${ANDROID_LOG_LIBS})
    elseif(APPLE)
        list(APPEND HID_DEPEND_LIBS "-framework IOKit" "-framework CoreFoundation" "-framework AppKit" "-framework CoreServices")
    endif()
    if (NOT APPLE)
        list(APPEND GEN_HID_SRC hidraw_driver.cpp)
    endif()
else()
    list(APPEND HID_DEPEND_LIBS hid setupapi)
endif()



add_library( ${TARGET_NAME} OBJECT ${GEN_HID_SRC} )

# Get the INTERFACE_COMPILE_OPTIONS property for the target framework::framework
get_target_property(FRAMEWORK_COMPILE_OPTIONS framework::framework INTERFACE_COMPILE_OPTIONS)
# Print the value of the INTERFACE_COMPILE_OPTIONS property
message(STATUS "framework::framework INTERFACE_COMPILE_OPTIONS: ${FRAMEWORK_COMPILE_OPTIONS}")

set_property(TARGET framework::framework PROPERTY INTERFACE_COMPILE_OPTIONS "")

target_link_libraries( ${TARGET_NAME} PUBLIC ${HID_DEPEND_LIBS} )
target_include_directories( ${TARGET_NAME} PUBLIC "$<BUILD_INTERFACE:${INCLUDE_DIRS}>" )

set(GENERIC_HID_DEPEND_LIBS ${HID_DEPEND_LIBS} ${TARGET_NAME} PARENT_SCOPE)

PROJECT_INSTALL(${TARGET_NAME} ${TARGETS_EXPORT_NAME})