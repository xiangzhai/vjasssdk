set(wc3lib_EDITOR_QOGRE_SRC                            
	QOgreWidget.cpp
)

set(wc3lib_EDITOR_QOGRE_H
	QOgreWidget.h
)

QT4_AUTOMOC(${wc3lib_EDITOR_QOGRE_SRC})
add_library(wc3libeditorqogre SHARED ${wc3lib_EDITOR_QOGRE_SRC})
target_link_libraries(wc3libeditorqogre OgreMain ${QT_LIBRARIES})

install(TARGETS wc3libeditorqogre DESTINATION lib/wc3lib/editor/qogre)
install(FILES ${wc3lib_EDITOR_QOGRE_H} PUBLIC_HEADER DESTINATION lib/wc3lib/editor/qogre)