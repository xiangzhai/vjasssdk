WC3LIB_ENABLE_EXCEPTIONS()

WC3LIB_GET_H(wc3lib_LIB_QOGRE_H)
WC3LIB_GET_SRC(wc3lib_LIB_QOGRE_SRC)

QT4_AUTOMOC(${wc3lib_LIB_QOGRE_SRC})
add_library(qogre SHARED ${wc3lib_LIB_QOGRE_SRC})
target_link_libraries(qogre OgreMain ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY})
# use QT_LIBRARIES

# install
install(TARGETS qogre DESTINATION lib/wc3lib/lib/)
install(FILES ${wc3lib_LIB_QOGRE_H} PUBLIC_HEADER DESTINATION include/wc3lib/lib/qogre)