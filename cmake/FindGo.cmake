include(FindPackageHandleStandardArgs)

find_program(Go_EXECUTABLE NAMES go)

find_package_handle_standard_args(Go REQUIRED_VARS Go_EXECUTABLE)

if(Go_FOUND)
  mark_as_advanced(Go_EXECUTABLE)
  set(Go_COMMAND ${Go_EXECUTABLE})
endif()
