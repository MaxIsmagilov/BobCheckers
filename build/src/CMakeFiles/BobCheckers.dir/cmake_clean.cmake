file(REMOVE_RECURSE
  "libBobCheckers.a"
  "libBobCheckers.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang CXX)
  include(CMakeFiles/BobCheckers.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
