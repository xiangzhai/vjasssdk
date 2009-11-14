find_package(Qt4)
find_package(KDE4)
find_package(Ogre)

include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
link_directories(${KDE4_LIB_DIR})
include_directories(${KDE4_INCLUDE_DIR} ${QT_INCLUDE_DIR} ${Ogre_INCLUDE_DIR})
message("Include dir" ${Ogre_INCLUDE_DIR})
include_directories(/usr/include/OGRE)

set(wc3lib_EDITOR_SRC
	blpiohandler.cpp
	blpioplugin.cpp
#	editor.cpp
#	modelview.cpp
#	renderer.cpp
#	terraineditor.cpp
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
)

#add_subdirectory(qogre)

QT4_WRAP_UI(wc3lib_EDITOR_UI_H ${wc3lib_EDITOR_UI})

add_library(wc3libeditor SHARED ${wc3lib_EDITOR_SRC})
#target_link_libraries(wc3libblp gettextlib)
 #OgreMain, qogre/wc3libeditorqogre
target_link_libraries(wc3libeditor wc3libblp ${QT_LIBRARIES} ${KDE4_KIO_LIBS} ${KDE4_KUTILS_LIBS} ${KDE4_KPARTS_LIBS})

#install

install(TARGETS wc3libeditor DESTINATION lib/wc3lib/)
install(FILES ${wc3lib_EDITOR_H} PUBLIC_HEADER DESTINATION lib/wc3lib/editor)