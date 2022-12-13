include(FindPackageHandleStandardArgs)

find_path(EGo_HOME
  NAMES bin/ego
  PATHS /opt/ego
  NO_DEFAULT_PATH
)

find_package_handle_standard_args(EGo REQUIRED_VARS EGo_HOME)

if(EGo_FOUND)
  mark_as_advanced(EGo_HOME)
  set(EGo_COMMAND ${EGo_HOME}/bin/ego)
  set(EGo_Go_COMMAND ${EGo_HOME}/bin/ego-go)
endif()
