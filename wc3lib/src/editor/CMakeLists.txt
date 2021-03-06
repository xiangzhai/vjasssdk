WC3LIB_ENABLE_EXCEPTIONS()
include_directories(${KDE4_INCLUDE_DIR} ${QT_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ) # include binary dir since the KDE configuration class will be generated there and has to be included (also include for moc cxx files)
add_subdirectory(ui)

include(${QT_USE_FILE})
WC3LIB_GET_H(wc3lib_EDITOR_H)
WC3LIB_GET_SRC(wc3lib_EDITOR_SRC)

list(REMOVE_ITEM wc3lib_EDITOR_SRC blpiohandler.cpp blpioplugin.cpp)
#kde4_add_kcfg_files(${wc3lib_EDITOR_SRC} configuration.kcfgc)


# BLP plugin
# allows you to read and write BLP image files by using class QImage and image type "BLP"
add_library(qblp SHARED blpiohandler.cpp blpioplugin.cpp)
target_link_libraries(qblp wc3libblp ${QT_LIBRARIES})

# wc3libeditor
message(STATUS "These are the editor source files:")
foreach (entry ${wc3lib_EDITOR_SRC})
	message(STATUS "${entry}")
endforeach ()
kde4_add_library(wc3libeditor SHARED ${wc3lib_EDITOR_SRC})
target_link_libraries(wc3libeditor ${wc3lib_CORE_LIBRARIES} ${GETTEXT_LIBRARIES} ${Boost_LIBRARIES} ${OGRE_LIBRARIES} ${QT_LIBRARIES} ${KDE4_KIO_LIBS} ${KDE4_KUTILS_LIBS} ${KDE4_KPARTS_LIBS})

# install
install(TARGETS qblp DESTINATION ${QT_PLUGINS_DIR}/imageformats)
install(TARGETS wc3libeditor DESTINATION lib/wc3lib/)
install(FILES ${wc3lib_EDITOR_H} PUBLIC_HEADER DESTINATION include/wc3lib/editor)
install(FILES editor.kcfg DESTINATION ${KCFG_INSTALL_DIR})