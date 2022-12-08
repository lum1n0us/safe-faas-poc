cmake_minimum_required(VERSION 3.14)

project(secure-faas-poc)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

#
# dependencies
#

# find go
# find tinygo
# find ego
# find openeclave
# find sgx

execute_process(
  COMMAND git submodule update --init wasm-micro-runtime
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/external)
