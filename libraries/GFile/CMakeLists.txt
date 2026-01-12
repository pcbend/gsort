
#this will compile the current library.
add_library(GFILE SHARED GFile.cxx)

target_include_directories(GFILE PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(GFILE PUBLIC "${CMAKE_SOURCE_DIR}/include")

#root_generate_dictionary(G__GTYPES 
#  Gtypes.h 
#  MODULE GTYPES 
#  LINKDEF Linkdef.h
#)

root_generate_dictionary(G__GFILE
    "${CMAKE_SOURCE_DIR}/include/GFile.h" # <- Explicit full path
    MODULE GFILE
    LINKDEF "${CMAKE_CURRENT_SOURCE_DIR}/Linkdef.h" # Explicit full path for Linkdef.h too
)

target_link_libraries(GFILE PUBLIC ROOT::Core GCORE GTYPES)


#ROOT::Core ROOT::ROOTDataFrame)

#link the compiled libraries above to the final executable.

