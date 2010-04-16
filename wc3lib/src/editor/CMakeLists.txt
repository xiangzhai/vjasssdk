WC3LIB_ENABLE_EXCEPTIONS()

set(wc3lib_EDITOR_SRC
	aieditor.cpp
	blpiohandler.cpp
	blpioplugin.cpp
	campaigneditor.cpp
	editor.cpp
	importmanager.cpp
	modeleditor.cpp
	modelview.cpp
	mpqeditor.cpp
	newmapdialog.cpp
	renderer.cpp
	objecteditor.cpp
	objectmanager.cpp
	soundeditor.cpp	
	subeditor.cpp
	terraineditor.cpp
	textureeditor.cpp
	triggereditor.cpp
)

set(wc3lib_EDITOR_H
	blpiohandler.hpp
	blpioplugin.hpp
	editor.hpp
	modelview.hpp
	renderer.hpp
	terraineditor.hpp
)

set(wc3lib_EDITOR_UI
	modeleditor.ui
	newmapdialog.ui
	triggereditor.ui
)

QT4_WRAP_UI(wc3lib_EDITOR_UI_H ${wc3lib_EDITOR_UI})

add_library(wc3libeditor SHARED ${wc3lib_EDITOR_SRC})
# todo use OGRE_LIBRARIES from custom cmake module
target_link_libraries(wc3libeditor wc3libcore wc3libblp wc3libmpq wc3libmdlx qogre ${GETTEXT_LIBRARIES} ${Boost_LIBRARIES} OgreMain ${QT_LIBRARIES} ${KDE4_KIO_LIBS} ${KDE4_KUTILS_LIBS} ${KDE4_KPARTS_LIBS})

# install

install(TARGETS wc3libeditor DESTINATION lib/wc3lib/)
install(FILES ${wc3lib_EDITOR_H} PUBLIC_HEADER DESTINATION include/wc3lib/editor)