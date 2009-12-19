find_package(Qt4 REQUIRED)
find_package(KDE4 REQUIRED)
find_library(Ogre REQUIRED)

include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
link_directories(${KDE4_LIB_DIR})
include_directories(${KDE4_INCLUDE_DIR} ${QT_INCLUDE_DIR} ${Ogre_INCLUDE_DIR})
message("Include dir" ${Ogre_INCLUDE_DIR})
include_directories(/usr/include/OGRE)

set(wc3lib_EDITOR_SRC
	#aieditor.cpp
	blpiohandler.cpp
	blpioplugin.cpp
	#campaigneditor.cpp
	#editor.cpp
	#importmanager.cpp
	#modeleditor.cpp
	#modelview.cpp
	#mpqeditor.cpp
	#newmapdialog.cpp
#	renderer.cpp
	#objecteditor.cpp
	#objectmanager.cpp
	#soundeditor.cpp	
	#subeditor.cpp
	#terraineditor.cpp
	#textureeditor.cpp
	#triggereditor.cpp
)

set(wc3lib_EDITOR_H
	blpiohandler.hpp
	blpioplugin.hpp
	editor.hpp
	modelview.hpp
#	renderer.hpp
	terraineditor.hpp
)

set(wc3lib_EDITOR_UI
	modeleditor.ui
	newmapdialog.ui
	triggereditor.ui
)

#add_subdirectory(qogre)

QT4_WRAP_UI(wc3lib_EDITOR_UI_H ${wc3lib_EDITOR_UI})

add_library(wc3libeditor SHARED ${wc3lib_EDITOR_SRC})
target_link_libraries(wc3libeditor wc3libblp OgreMain ${QT_LIBRARIES} ${KDE4_KIO_LIBS} ${KDE4_KUTILS_LIBS} ${KDE4_KPARTS_LIBS})
#wc3libeditorqogre

#install

install(TARGETS wc3libeditor DESTINATION lib/wc3lib/)
install(FILES ${wc3lib_EDITOR_H} PUBLIC_HEADER DESTINATION include/wc3lib/editor)
