include(FindPackageHandleStandardArgs)

find_program(TinyGo_EXECUTABLE NAMES tinygo)

find_package_handle_standard_args(TinyGo REQUIRED_VARS TinyGo_EXECUTABLE)

if(TinyGo_FOUND)
  mark_as_advanced(TinyGo_EXECUTABLE)
  set(TinyGo_COMMAND ${TinyGo_EXECUTABLE})
endif()
