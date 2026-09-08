# generated from ament/cmake/core/templates/nameConfig.cmake.in

# prevent multiple inclusion
if(_tutoria_1_set_CONFIG_INCLUDED)
  # ensure to keep the found flag the same
  if(NOT DEFINED tutoria_1_set_FOUND)
    # explicitly set it to FALSE, otherwise CMake will set it to TRUE
    set(tutoria_1_set_FOUND FALSE)
  elseif(NOT tutoria_1_set_FOUND)
    # use separate condition to avoid uninitialized variable warning
    set(tutoria_1_set_FOUND FALSE)
  endif()
  return()
endif()
set(_tutoria_1_set_CONFIG_INCLUDED TRUE)

# output package information
if(NOT tutoria_1_set_FIND_QUIETLY)
  message(STATUS "Found tutoria_1_set: 0.0.0 (${tutoria_1_set_DIR})")
endif()

# warn when using a deprecated package
if(NOT "" STREQUAL "")
  set(_msg "Package 'tutoria_1_set' is deprecated")
  # append custom deprecation text if available
  if(NOT "" STREQUAL "TRUE")
    set(_msg "${_msg} ()")
  endif()
  # optionally quiet the deprecation message
  if(NOT tutoria_1_set_DEPRECATED_QUIET)
    message(DEPRECATION "${_msg}")
  endif()
endif()

# flag package as ament-based to distinguish it after being find_package()-ed
set(tutoria_1_set_FOUND_AMENT_PACKAGE TRUE)

# include all config extra files
set(_extras "")
foreach(_extra ${_extras})
  include("${tutoria_1_set_DIR}/${_extra}")
endforeach()
