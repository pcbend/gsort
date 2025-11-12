
#this will compile the current library.
add_library(GTHREAD SHARED 
  GThread.cxx 
  GFileInput.cxx
  GBanks.cxx
  GEvent.cxx
  GEBFile.cxx
  GEBEvent.cxx
)

root_generate_dictionary(G__GTHREAD 
  GThread.h 
  GFileInput.h 
  GEBFile.h
  GEvent.h
  GEBEvent.h
  MODULE GTHREAD 
  LINKDEF Linkdef.h
)

target_link_libraries(GTHREAD GTYPES ${ROOT_LIBRARIES})

#link the compiled libraries above to the final executable.

