
#this will compile the current library.
add_library(GGRETINA SHARED GBank88.cxx GMode3Hit.cxx)

target_include_directories(GGRETINA PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(GGRETINA PUBLIC "${CMAKE_SOURCE_DIR}/include")

#root_generate_dictionary(G__GTYPES 
#  Gtypes.h 
#  MODULE GTYPES 
#  LINKDEF Linkdef.h
#)

root_generate_dictionary(G__GGRETINA
    #"${CMAKE_SOURCE_DIR}/include/GDetector.h" # <- Explicit full path
    "${CMAKE_SOURCE_DIR}/include/GBank88.h" # <- Explicit full path
    "${CMAKE_SOURCE_DIR}/include/GMode3Hit.h" # <- Explicit full path
    MODULE GGRETINA
    LINKDEF "${CMAKE_CURRENT_SOURCE_DIR}/Linkdef.h" # Explicit full path for Linkdef.h too
)



target_link_libraries(GGRETINA PUBLIC ROOT::Core GDETECTOR)

#link the compiled libraries above to the final executable.

