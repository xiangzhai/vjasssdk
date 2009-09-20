set(wc3lib_MAP_SRC
	w3m.cpp
)

set(wc3lib_MAP_H
	environment.hpp
	itemset.hpp
	pathmap.hpp
	platform.hpp
	shadow.hpp
	tilepoint.hpp
	tree.hpp
	trees.hpp
	w3m.hpp
	w3x.hpp
)

add_library(wc3libmap SHARED ${wc3lib_MAP_SRC})
#target_link_libraries(wc3libblp gettextlib)

#install

install(TARGETS wc3libmap DESTINATION lib/wc3lib/)
install(FILES ${wc3lib_MAP_H} PUBLIC_HEADER DESTINATION lib/wc3lib/map)