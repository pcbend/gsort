
add_library(GPIPELINE SHARED
  GPipeline.cxx
)

target_include_directories(GPIPELINE
  PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${ROOT_INCLUDE_DIRS}
)

target_link_libraries(GPIPELINE
  PUBLIC
    GFILE
    GTYPES
    GDETECTOR
    UNPACKER
  PRIVATE
    GHISTOGRAM
    GGRETINA
    ROOT::Core
    ROOT::Tree
)
