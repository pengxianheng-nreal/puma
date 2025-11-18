cmake_minimum_required( VERSION 3.25.3 )

project(puma
        VERSION 1.0.0
        DESCRIPTION "Puma project for nrsdk!"
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

# After set environment variable, we includ project cmake.
include(Project)

# Set git hook
execute_process(COMMAND git config core.hooksPath external/project/githook)
