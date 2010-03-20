add_executable(converter converter.cpp)
target_link_libraries(converter ${wc3lib_LIBRARIES} ${Boost_LIBRARIES})
message("These are the Boost libraries: ${Boost_LIBRARIES}.")

#kde4_add_executable(editor editor.cpp)
#add_executable(editor editor.cpp)
#target_link_libraries(editor ${wc3lib_LIBRARIES} ${KDE4_KIO_LIBS} ${KDE4_KUTILS_LIBS} ${KDE4_KPARTS_LIBS})

#add_executable(jassc jassc.cpp)
#target_link_libraries(jassc ${wc3lib_LIBRARIES} ${Boost_LIBRARIES})

#add_executable(tilesetter tilesetter.cpp)
#target_link_libraries(tilesetter ${wc3lib_LIBRARIES} ${Boost_LIBRARIES})
 
add_executable(mpq mpq.cpp)
target_link_libraries(mpq ${wc3lib_LIBRARIES} ${Boost_LIBRARIES})

#install

#install(TARGETS converter jassc tilesetter mpq DESTINATION bin/wc3lib)
