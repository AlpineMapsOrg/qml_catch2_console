cmake_minimum_required(VERSION 3.21)

project(qml_catch2_console)
set(ALP_EXTERN_DIR "external" CACHE STRING "name of the directory to store external libraries, fonts etc..")
option(ALP_ENABLE_ADDRESS_SANITIZER "compiles atb with address sanitizer enabled (only debug, works only on g++ and clang)" OFF)
if (EMSCRIPTEN)
    set(ALP_WWW_INSTALL_DIR "${CMAKE_CURRENT_BINARY_DIR}" CACHE PATH "path to the install directory (for webassembly files, i.e., www directory)")
endif()

include(cmake/alp_add_git_repository.cmake)
add_subdirectory(unittests)
