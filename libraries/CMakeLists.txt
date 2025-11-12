
add_subdirectory(Gtypes)


add_subdirectory(Gint)
target_link_libraries(gsort Gint)        

add_subdirectory(GFile)
target_link_libraries(Gint GFILE)        

add_subdirectory(GEBDataSource)
target_link_libraries(Gint GEBDATASOURCE)

add_subdirectory(GThread)
target_link_libraries(Gint GTHREAD)



