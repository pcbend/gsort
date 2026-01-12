
#this will compile the current library.
add_library(GHISTOGRAM SHARED GHistogramer.cxx)

root_generate_dictionary(G__GHistogram GHistogramer.h MODULE GHISTOGRAM LINKDEF Linkdef.h)

target_link_libraries(GHISTOGRAM GCORE ${ROOT_LIBRARIES})


#link the compiled libraries above to the final executable.

