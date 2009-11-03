find_package(Qt4)
find_package(KDE4)

include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
link_directories(${KDE4_LIB_DIR})
include_directories(${KDE4_INCLUDE_DIR} ${QT_INCLUDE_DIR})
include_directories(/usr/include/OGRE)

set(wc3lib_EDITOR_SRC
	editor.cpp
	renderer.cpp
)

set(wc3lib_EDITOR_H
	editor.hpp
	renderer.hpp
)

add_library(wc3libeditor SHARED ${wc3lib_EDITOR_SRC})
#target_link_libraries(wc3libblp gettextlib)
target_link_libraries(wc3libeditor OgreMain ${QT_LIBRARIES} ${KDE4_KIO_LIBS} ${KDE4_KUTILS_LIBS} ${KDE4_KPARTS_LIBS})

#install

install(TARGETS wc3libeditor DESTINATION lib/wc3lib/)
install(FILES ${wc3lib_EDITOR_H} PUBLIC_HEADER DESTINATION lib/wc3lib/editor)