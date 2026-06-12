

#this will compile the current library.
add_library(GHISTOGRAM SHARED 
  GHistogramer.cxx
)


target_include_directories(GHISTOGRAM
  #PRIVATE
  PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${ROOT_INCLUDE_DIRS}
) 

root_generate_dictionary(G__GHistogram 
  GHistogramer.h 
  MODULE GHISTOGRAM 
  LINKDEF Linkdef.h
)

target_link_libraries(GHISTOGRAM PUBLIC ${ROOT_LIBRARIES})

#link the compiled libraries above to the final executable.

