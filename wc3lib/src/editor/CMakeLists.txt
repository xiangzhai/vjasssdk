set(wc3lib_EDITOR_SRC                                 
	renderer.cpp
)

set(wc3lib_EDITOR_H
	renderer.hpp
)

add_library(wc3libeditor SHARED ${wc3lib_EDITOR_SRC})
#target_link_libraries(wc3libblp gettextlib)
target_link_libraries(wc3libeditor ogre)

#install

install(TARGETS wc3libeditor DESTINATION lib/wc3lib/)
install(FILES ${wc3lib_EDITOR_H} PUBLIC_HEADER DESTINATION lib/wc3lib/editor)