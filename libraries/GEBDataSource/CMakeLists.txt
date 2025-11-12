
#this will compile the current library.
add_library(GEBDATASOURCE SHARED GEBDataSource.cxx)

target_include_directories(GEBDATASOURCE PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(GEBDATASOURCE PUBLIC "${CMAKE_SOURCE_DIR}/include")

#root_generate_dictionary(G__GTYPES 
#  Gtypes.h 
#  MODULE GTYPES 
#  LINKDEF Linkdef.h
#)

root_generate_dictionary(G__GEBDATASOURCE
    "${CMAKE_SOURCE_DIR}/include/GEBDataSource.h" # <- Explicit full path
    MODULE GEBDATASOURCE
    LINKDEF "${CMAKE_CURRENT_SOURCE_DIR}/Linkdef.h" # Explicit full path for Linkdef.h too
)



target_link_libraries(GEBDATASOURCE PUBLIC ROOT::Core ROOT::ROOTDataFrame)

#link the compiled libraries above to the final executable.

