
#this will compile the current library.
add_library(Gint SHARED Gint.cxx GCommands.cxx)

root_generate_dictionary(G__Gint Gint.h Gtypes.h GCommands.h MODULE Gint LINKDEF Linkdef.h)

target_link_libraries(Gint GFILE GCORE ${ROOT_LIBRARIES})



#link the compiled libraries above to the final executable.

