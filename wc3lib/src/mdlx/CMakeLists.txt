set(wc3lib_MDLX_SRC
	attachment.cpp
	bone.cpp
	camera.cpp
	collisionshape.cpp
	event.cpp
	ganimation.cpp
	geoset.cpp
	geosetanimation.cpp
	mdlx.cpp
	object.cpp
	visibility.cpp
)

set(wc3lib_MDLX_H
	attachment.hpp
	bone.hpp
	camera.hpp
	collisionshape.hpp
	event.hpp
	ganimation.hpp
	geoset.hpp
	geosetanimation.hpp
	mdlx.hpp
	object.hpp
	visibility.hpp
)

add_library(wc3libmdlx SHARED ${wc3lib_MDLX_SRC})
#target_link_libraries(wc3libblp gettextlib)

#install

install(TARGETS wc3libmdlx DESTINATION lib/wc3lib/)
install(FILES ${wc3lib_MDLX_H} PUBLIC_HEADER DESTINATION lib/wc3lib/mdlx)