
#this will compile the current library.
add_library(GDETECTOR SHARED GDetector.cxx)

target_include_directories(GDETECTOR PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(GDETECTOR PUBLIC "${CMAKE_SOURCE_DIR}/include")

#root_generate_dictionary(G__GTYPES 
#  Gtypes.h 
#  MODULE GTYPES 
#  LINKDEF Linkdef.h
#)

root_generate_dictionary(G__GDETECTOR
    "${CMAKE_SOURCE_DIR}/include/GDetector.h" # <- Explicit full path
    MODULE GDETECTOR
    LINKDEF "${CMAKE_CURRENT_SOURCE_DIR}/Linkdef.h" # Explicit full path for Linkdef.h too
)



target_link_libraries(GDETECTOR PUBLIC ROOT::Core)

#link the compiled libraries above to the final executable.

