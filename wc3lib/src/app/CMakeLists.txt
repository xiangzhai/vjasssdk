add_executable(converter converter.cpp)
target_link_libraries(converter wc3lib ${Boost_LIBRARIES})

kde4_add_executable(editor editor.cpp)
#add_executable(editor editor.cpp)
target_link_libraries(editor wc3lib ${KDE4_KIO_LIBS} ${KDE4_KUTILS_LIBS} ${KDE4_KPARTS_LIBS})

add_executable(jassc jassc.cpp)
target_link_libraries(jassc wc3lib ${Boost_LIBRARIES})

add_executable(tilesetter tilesetter.cpp)
target_link_libraries(tilesetter wc3lib ${Boost_LIBRARIES})

#install

install(TARGETS converter jassc tilesetter DESTINATION bin/wc3lib)
