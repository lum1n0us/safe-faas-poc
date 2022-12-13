include(FindPackageHandleStandardArgs)

file(GLOB WABT_SEARCH_PATH "/opt/wabt-*")
find_path(WABT_HOME
  NAMES bin/wasm2wat
  PATHS ${WABT_SEARCH_PATH}
  NO_DEFAULT_PATH
  REQUIRED
)

string(REGEX MATCH [0-9]+\.[0-9]+\.*[0-9]* WABT_VERSION ${WABT_HOME})

find_package_handle_standard_args(WABT REQUIRED_VARS WABT_HOME VERSION_VAR WABT_VERSION)

if(WABT_FOUND)
  mark_as_advanced(WABT_HOME)
  set(WABT_WASM_OBJDUMP_COMMAND ${WABT_HOME}/bin/wasm-objdump)
endif()
