WC3LIB_ENABLE_EXCEPTIONS()

add_subdirectory(ui)

WC3LIB_GET_H(wc3lib_EDITOR_H)
WC3LIB_GET_SRC(wc3lib_EDITOR_SRC)

QT4_WRAP_CPP(wc3lib_EDITOR_MOC_SRC ${wc3lib_EDITOR_H})

#QT4_GENERATE_MOC(inputfile outputfile )
#QT4_AUTOMOC(${wc3lib_EDITOR_MOC_SRC})

# get SRC again (new moc files)
#set(wc3lib_EDITOR_SRC "")
#WC3LIB_GET_SRC(wc3lib_EDITOR_SRC)
#message(STATUS "This is SRC ${wc3lib_EDITOR_SRC}")

# BLP plugin
add_library(qblp SHARED blpiohandler.cpp blpioplugin.cpp)
target_link_libraries(qblp wc3libblp ${QT_LIBRARIES})
add_library(wc3libeditor SHARED ${wc3lib_EDITOR_SRC})
target_link_libraries(wc3libeditor ${wc3lib_CORE_LIBRARIES} ${GETTEXT_LIBRARIES} ${Boost_LIBRARIES} ${OGRE_LIBRARIES} ${QT_LIBRARIES} ${KDE4_KIO_LIBS} ${KDE4_KUTILS_LIBS} ${KDE4_KPARTS_LIBS})

# install
install(TARGETS qblp DESTINATION lib/qt4/plugins/imageformats)
install(TARGETS wc3libeditor DESTINATION lib/wc3lib/)
install(FILES ${wc3lib_EDITOR_H} PUBLIC_HEADER DESTINATION include/wc3lib/editor)