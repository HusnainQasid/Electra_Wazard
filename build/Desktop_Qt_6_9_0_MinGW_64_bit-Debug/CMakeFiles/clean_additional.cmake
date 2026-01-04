# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\Electrical_House_Wiring_System_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\Electrical_House_Wiring_System_autogen.dir\\ParseCache.txt"
  "Electrical_House_Wiring_System_autogen"
  )
endif()
