cmake_minimum_required(VERSION 3.14)

project(secure-faas-poc)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

#
# dependencies
#
find_package(OpenEnclave)
find_package(SGX)
find_package(Go REQUIRED)
find_package(TinyGo REQUIRED)
find_package(EGo REQUIRED)
find_package(WASISDK 16.0 REQUIRED)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  find_package(WABT 1.0.31 REQUIRED)
endif()

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
# init fd table failed
set(WAMR_BUILD_LIBC_WASI 0)

add_subdirectory(${PROJECT_SOURCE_DIR}/external/wasm-micro-runtime/ EXCLUDE_FROM_ALL)

add_library(wamr STATIC ${PROJECT_SOURCE_DIR}/library/src/glue.c)
target_include_directories(
  wamr
  PUBLIC
    ${PROJECT_SOURCE_DIR}/library/include/
    ${PROJECT_SOURCE_DIR}/external/wasm-micro-runtime/core/iwasm/include/
)

#
# the Go server
#
add_custom_command(
  OUTPUT go_server
  DEPENDS ${PROJECT_SOURCE_DIR}/app/server/main.go iwasm_static
  COMMAND  openssl genrsa -out private.pem -3 3072
            && ${CMAKE_COMMAND} -E rm -f ./go_server
            && ${EGo_Go_COMMAND} build -o go_server ${PROJECT_SOURCE_DIR}/app/server/main.go
            && ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/app/server/config.json config.json
            && ${CMAKE_COMMAND} -E make_directory data/wasm_modules/
            && ${CMAKE_COMMAND} -E copy c_faas.wasm data/wasm_modules/
            && ${EGo_COMMAND} sign config.json
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
add_custom_target(goserverbuild ALL DEPENDS go_server)
add_dependencies(goserverbuild wamr iwasm_static cwasmbuild)

#
# Application: Go -> Wasm
#
add_custom_command(
  OUTPUT go_faas.wasm
  DEPENDS ${PROJECT_SOURCE_DIR}/app/wasm/main.go
  COMMAND ${TinyGo_COMMAND} build --target wasi -o go_faas.wasm ${PROJECT_SOURCE_DIR}/app/wasm/main.go
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
add_custom_target(gowasmbuild ALL DEPENDS go_faas.wasm)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  # need to use dump information to confirm exported item indexes
  add_custom_command(
    OUTPUT go_faas.dump
    DEPENDS go_faas.wasm
    COMMAND ${WABT_WASM_OBJDUMP_COMMAND} -dx go_faas.wasm > go_faas.dump
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
  add_custom_target(gowasmdumpbuild ALL DEPENDS go_faas.dump)
endif()

#
# Application: C -> Wasm
#
add_custom_command(
  OUTPUT c_faas.wasm
  DEPENDS ${PROJECT_SOURCE_DIR}/app/wasm/main.c
  COMMAND /opt/wasi-sdk-16.0/bin/clang --target=wasm32-wasi --sysroot=/opt/wasi-sdk-16.0/share/wasi-sysroot/ -mexec-model=reactor -mbulk-memory -Wl,--export=malloc,--export=free ${PROJECT_SOURCE_DIR}/app/wasm/main.c -o c_faas.wasm
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
add_custom_target(cwasmbuild ALL DEPENDS c_faas.wasm)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  # need to use dump information to confirm exported item indexes
  add_custom_command(
    OUTPUT c_faas.dump
    DEPENDS c_faas.wasm
    COMMAND ${WABT_WASM_OBJDUMP_COMMAND} -dx c_faas.wasm > c_faas.dump
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
  add_custom_target(cwasmdumpbuild ALL DEPENDS c_faas.dump)
endif()


#
# Install
#

#
# Tests
#
enable_testing()

add_test(
  NAME standalone-test
  COMMAND ../go_server
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/data
)

add_test(
  NAME smoking-test
  COMMAND ${CMAKE_COMMAND} -E env OE_SIMULATION=1 ${EGo_COMMAND} run go_server
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)