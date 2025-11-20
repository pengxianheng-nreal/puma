cmake_minimum_required( VERSION 3.25.3 )

project(puma
        VERSION 1.0.0
        DESCRIPTION "Puma project for xreal SDK."
        LANGUAGES CXX C
        )

# Define relative dir for ${CMAKE_SOURCE_DIR}
set(PROJECT_SRC_DIR "puma")

find_package(framework CONFIG REQUIRED)
find_package(doctest CONFIG REQUIRED)
######################################################################################
##                              Compiler											##
######################################################################################
# set(CMAKE_C_FLAGS " xxx")
# set(CMAKE_CXX_FLAGS " xxx")

######################################################################################
##                              Config Project                                      ##
######################################################################################
set(EXTERNAL_PROJECT_DIR ${PROJECT_SOURCE_DIR}/external/project)
list( APPEND CMAKE_MODULE_PATH "${EXTERNAL_PROJECT_DIR}/cmake" )

if(NOT IOS)
	set(EXTERNAL_INCLUDE_LIST "${CMAKE_CURRENT_SOURCE_DIR}/external/libyuv/include")
	if (NOT WIN32 AND NOT APPLE)
		list(APPEND EXTERNAL_INCLUDE_LIST "${CMAKE_CURRENT_SOURCE_DIR}/external/libuvc/include" "${CMAKE_CURRENT_SOURCE_DIR}/external/libusb/libusb")
	endif()
	set(EXTERNAL_INCLUDE_DIR ${EXTERNAL_INCLUDE_LIST})
endif()

# After set environment variable, we includ project cmake.
include(Project)

if(NOT IOS)
	######################################################################################
	##  							EXTERNAL  											##
	######################################################################################
	# External dependencies.
	add_subdirectory(external)
endif()

# Set git hook
execute_process(COMMAND git config core.hooksPath external/project/githook)
