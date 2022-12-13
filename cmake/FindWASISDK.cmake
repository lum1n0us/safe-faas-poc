include(FindPackageHandleStandardArgs)

file(GLOB WASISDK_SEARCH_PATH "/opt/wasi-sdk-*")
find_path(WASISDK_HOME
  NAMES share/wasi-sysroot
  PATHS ${WASISDK_SEARCH_PATH}
  NO_DEFAULT_PATH
  REQUIRED
)

string(REGEX MATCH [0-9]+\.[0-9]+\.*[0-9]* WASISDK_VERSION ${WASISDK_HOME})

find_package_handle_standard_args(WASISDK REQUIRED_VARS WASISDK_HOME VERSION_VAR WASISDK_VERSION)

if(WASISDK_FOUND)
  mark_as_advanced(WASISDK_HOME)
  set(WASISDK_CC_COMMAND {WASISDK_HOME}/bin/clang)
  set(WASISDK_CXX_COMMAND {WASISDK_HOME}/bin/clang++)
  set(WASISDK_SYSROOT {WASISDK_HOME}/share/wasi-sysroot)
endif()
