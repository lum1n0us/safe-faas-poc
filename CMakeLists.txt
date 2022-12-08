cmake_minimum_required(VERSION 3.14)

project(secure-faas-poc)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

#
# dependencies
#
find_package(Go)
find_package(TinyGo)
find_package(EGo)
find_package(OpenEnclave)
find_package(SGX)

execute_process(
  COMMAND git submodule update --init wasm-micro-runtime
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/external)

#
# global options
#
# -pedantic?
add_compile_options(-Wall -Wextra  -Werror)

#
# iwasm_static
#
set(WAMR_BUILD_AOT 0)
set(WAMR_BUILD_SIMD 0)
set(WAMR_BUILD_LIBC_BUILTIN 0)

add_subdirectory(external/wasm-micro-runtime)
add_custom_target(wamr_static_library)
add_dependencies(wamr_static_library iwasm_static)
