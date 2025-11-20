set(EXPORT_BASENAME PUMA)
set(PLATFORM_CONFIG_BASENAME PUMA)
set(SYSTEM_CONFIG_BASENAME PUMA)
set(VERSION_BASENAME PUMA)

add_compile_definitions(NRPLUGIN)

if(BUILD_SHARED_LIBS)
	add_compile_definitions(${EXPORT_BASENAME}_SHARED_LIBS ${EXPORT_BASENAME}_SHARD_LIBS_EXPORTS)
endif()

# debug check
if ( BUILD_TESTS OR BUILD_APPS OR BUILD_SINGLE)
	add_compile_definitions(NO_DEBUG_CHECK)
	add_compile_definitions(BUILD_FOR_BIN)
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")

set( TARGET_BASE_NAME puma )


set(DEPEND_LIBS)

if (APPLE)
list(APPEND DEPEND_LIBS  "-framework IOKit" "-framework CoreFoundation" "-framework AppKit" "-framework CoreServices" "-framework SecurityFoundation")
endif()

add_subdirectory(../external/project/cmake/env env)
add_subdirectory(interface)
add_subdirectory(library)
add_subdirectory(source)

if(NOT IOS)
	add_subdirectory(common)
	add_subdirectory(driver)
	add_subdirectory(util)
	add_subdirectory(provider)
	if(ANDROID)
		set(ANDROID_LOG_LIBS "log")
	endif()
else()
	add_subdirectory(dummy_interface)
endif()

message(STATUS "Puma: ${TARGET_BASE_NAME} ${DEPEND_LIBS}")

ADD_PLUGIN_LIBRARIES(${TARGET_BASE_NAME} shared_library static_library source provider ${DEPEND_LIBS})
