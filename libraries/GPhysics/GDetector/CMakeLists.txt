
#this will compile the current library.
add_library(GDETECTOR SHARED 
  GDetector.cxx 
  GDetectorHit.cxx
)

target_include_directories(GDETECTOR
  PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${ROOT_INCLUDE_DIRS}
) 



root_generate_dictionary(G__GDETECTOR
    GDetector.h
    GDetectorHit.h
    MODULE GDETECTOR
    LINKDEF Linkdef.h
)

target_link_libraries(GDETECTOR 
  PUBLIC 
    ROOT::Core 
)
  #GTYPES GHISTOGRAM)

#link the compiled libraries above to the final executable.

