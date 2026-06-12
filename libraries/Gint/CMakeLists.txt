
#this will compile the current library.
add_library(GINT SHARED 
  Gint.cxx 
  GCommands.cxx
)

target_include_directories(GINT
  PRIVATE
  ${CMAKE_SOURCE_DIR}/include
  ${ROOT_INCLUDE_DIRS}
) 

root_generate_dictionary(G__Gint 
  Gint.h 
  Gtypes.h 
  GCommands.h 
  MODULE GINT
  LINKDEF Linkdef.h
)

target_link_libraries(GINT 
  PUBLIC
    ROOT::Core
    ROOT::Rint
  PRIVATE 
  GPIPELINE
)



#link the compiled libraries above to the final executable.

