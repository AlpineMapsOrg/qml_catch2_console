cmake_minimum_required(VERSION 3.21)

project(qml_catch2_console)
set(ALP_EXTERN_DIR "external" CACHE STRING "name of the directory to store external libraries, fonts etc..")

include(cmake/alp_add_git_repository.cmake)
add_subdirectory(unittests)
