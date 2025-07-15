# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Release")
  file(REMOVE_RECURSE
  "CMakeFiles\\Local32Amplifer_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\Local32Amplifer_autogen.dir\\ParseCache.txt"
  "Local32Amplifer_autogen"
  )
endif()
