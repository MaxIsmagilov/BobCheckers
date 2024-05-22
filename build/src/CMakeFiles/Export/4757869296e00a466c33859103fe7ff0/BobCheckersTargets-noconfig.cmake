#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "BobCheckers::BobCheckers" for configuration ""
set_property(TARGET BobCheckers::BobCheckers APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(BobCheckers::BobCheckers PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_NOCONFIG "CXX"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libBobCheckers.a"
  )

list(APPEND _cmake_import_check_targets BobCheckers::BobCheckers )
list(APPEND _cmake_import_check_files_for_BobCheckers::BobCheckers "${_IMPORT_PREFIX}/lib/libBobCheckers.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
