
#this will compile the current library.
add_library(GTYPES SHARED Gtypes.cxx)

target_include_directories(GTYPES PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(GTYPES PUBLIC "${CMAKE_SOURCE_DIR}/include")

#root_generate_dictionary(G__GTYPES 
#  Gtypes.h 
#  MODULE GTYPES 
#  LINKDEF Linkdef.h
#)

root_generate_dictionary(G__GTYPES
    "${CMAKE_SOURCE_DIR}/include/Gtypes.h" # <- Explicit full path
    MODULE GTYPES
    LINKDEF "${CMAKE_CURRENT_SOURCE_DIR}/Linkdef.h" # Explicit full path for Linkdef.h too
)



target_link_libraries(GTYPES PUBLIC ROOT::Core)

#link the compiled libraries above to the final executable.

