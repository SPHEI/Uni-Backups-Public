# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Release")
  file(REMOVE_RECURSE
  "CMakeFiles\\Pesels_final_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\Pesels_final_autogen.dir\\ParseCache.txt"
  "Pesels_final_autogen"
  )
endif()
