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
# too strict for now
#add_compile_options(-Wall -Wextra -pedantic -Werror)

#
# the glue layer and WAMR
#
set(WAMR_BUILD_AOT 0)
set(WAMR_BUILD_SIMD 0)
set(WAMR_BUILD_LIBC_BUILTIN 0)

add_subdirectory(${PROJECT_SOURCE_DIR}/external/wasm-micro-runtime/)

add_library(wamr STATIC ${PROJECT_SOURCE_DIR}/library/src/glue.c)
target_include_directories(
  wamr
  PUBLIC
    ${PROJECT_SOURCE_DIR}/library/include
    ${PROJECT_SOURCE_DIR}/external/wasm-micro-runtime/core/iwasm/include
)
target_link_libraries(wamr iwasm_static)

#
# the Go application
#
add_custom_command(
  OUTPUT go_app.go
  DEPENDS ${PROJECT_SOURCE_DIR}/app/server/main.go
  COMMAND ego-go build -o go_app.go ${PROJECT_SOURCE_DIR}/app/server/main.go && ego sign go_app.go
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
add_custom_target(goappbuild ALL DEPENDS go_app.go)

#
# Install
#

#
# Tests
#
enable_testing()
add_test(
  NAME smoking-test
  COMMAND ${CMAKE_COMMAND} -E env OE_SIMULATION=1 ego run go_app.go
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)