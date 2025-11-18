set(EXPORT_BASENAME EXAMPLE)
set(PLATFORM_CONFIG_BASENAME EXAMPLE)
set(SYSTEM_CONFIG_BASENAME EXAMPLE)
set(VERSION_BASENAME EXAMPLE)

add_compile_definitions(NRPLUGIN)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")

set( TARGET_BASE_NAME puma )
ADD_PLUGIN_LIBRARIES(${TARGET_BASE_NAME} shared_library static_library source)

add_subdirectory(../external/project/cmake/env env)
add_subdirectory(interface)
add_subdirectory(library)
add_subdirectory(source)
