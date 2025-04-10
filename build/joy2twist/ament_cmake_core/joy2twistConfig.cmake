# generated from ament/cmake/core/templates/nameConfig.cmake.in

# prevent multiple inclusion
if(_joy2twist_CONFIG_INCLUDED)
  # ensure to keep the found flag the same
  if(NOT DEFINED joy2twist_FOUND)
    # explicitly set it to FALSE, otherwise CMake will set it to TRUE
    set(joy2twist_FOUND FALSE)
  elseif(NOT joy2twist_FOUND)
    # use separate condition to avoid uninitialized variable warning
    set(joy2twist_FOUND FALSE)
  endif()
  return()
endif()
set(_joy2twist_CONFIG_INCLUDED TRUE)

# output package information
if(NOT joy2twist_FIND_QUIETLY)
  message(STATUS "Found joy2twist: 0.0.0 (${joy2twist_DIR})")
endif()

# warn when using a deprecated package
if(NOT "" STREQUAL "")
  set(_msg "Package 'joy2twist' is deprecated")
  # append custom deprecation text if available
  if(NOT "" STREQUAL "TRUE")
    set(_msg "${_msg} ()")
  endif()
  # optionally quiet the deprecation message
  if(NOT ${joy2twist_DEPRECATED_QUIET})
    message(DEPRECATION "${_msg}")
  endif()
endif()

# flag package as ament-based to distinguish it after being find_package()-ed
set(joy2twist_FOUND_AMENT_PACKAGE TRUE)

# include all config extra files
set(_extras "")
foreach(_extra ${_extras})
  include("${joy2twist_DIR}/${_extra}")
endforeach()
