WC3LIB_ENABLE_EXCEPTIONS()

WC3LIB_GET_H(wc3lib_EDITOR_H)
WC3LIB_GET_SRC(wc3lib_EDITOR_SRC)
WC3LIB_GET_UI(wc3lib_EDITOR_UI)

#QT4_WRAP_CPP(MOC_SRCS ${INC_FILES})
#QT4_WRAP_UI(UI_SRCS ${UI_FILES})
#QT4_WRAP_UI(wc3lib_EDITOR_UI_H ${wc3lib_EDITOR_UI})

# BLP plugin
add_library(qblp SHARED blpiohandler.cpp blpioplugin.cpp)
target_link_libraries(qblp wc3libblp ${QT_LIBRARIES})
add_library(wc3libeditor SHARED ${wc3lib_EDITOR_SRC})
# todo use OGRE_LIBRARIES from custom cmake module
target_link_libraries(wc3libeditor wc3libcore wc3libblp wc3libmpq wc3libmdlx qogre ${GETTEXT_LIBRARIES} ${Boost_LIBRARIES} OgreMain ${QT_LIBRARIES} ${KDE4_KIO_LIBS} ${KDE4_KUTILS_LIBS} ${KDE4_KPARTS_LIBS})

# install
install(TARGETS qblp DESTINATION lib/qt4/plugins/imageformats)
#install(TARGETS wc3libeditor DESTINATION lib/wc3lib/)
#install(FILES ${wc3lib_EDITOR_H} PUBLIC_HEADER DESTINATION include/wc3lib/editor)