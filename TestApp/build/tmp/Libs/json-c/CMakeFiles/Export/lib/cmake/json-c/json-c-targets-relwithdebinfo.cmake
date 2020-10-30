#----------------------------------------------------------------
# Generated CMake target import file for configuration "RelWithDebInfo".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "json-c::json-c" for configuration "RelWithDebInfo"
set_property(TARGET json-c::json-c APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(json-c::json-c PROPERTIES
  IMPORTED_IMPLIB_RELWITHDEBINFO "${_IMPORT_PREFIX}/lib/json-c.lib"
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/bin/json-c.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS json-c::json-c )
list(APPEND _IMPORT_CHECK_FILES_FOR_json-c::json-c "${_IMPORT_PREFIX}/lib/json-c.lib" "${_IMPORT_PREFIX}/bin/json-c.dll" )

# Import target "json-c::json-c-static" for configuration "RelWithDebInfo"
set_property(TARGET json-c::json-c-static APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(json-c::json-c-static PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELWITHDEBINFO "C"
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/lib/json-c-static.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS json-c::json-c-static )
list(APPEND _IMPORT_CHECK_FILES_FOR_json-c::json-c-static "${_IMPORT_PREFIX}/lib/json-c-static.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
