set(wc3lib_BLP_SRC
	blp.cpp
	blpuncompressed1.cpp
	blpuncompressed2.cpp
)

set(wc3lib_BLP_H
	blp.hpp
	blpjpeg.hpp
	blpuncompressed1.hpp
	blpuncompressed2.hpp
	platform.hpp
)

add_library(wc3libblp SHARED ${wc3lib_BLP_SRC})
#target_link_libraries(wc3libblp gettextlib)

#install

install(TARGETS wc3libblp DESTINATION lib/wc3lib/)
install(FILES ${wc3lib_BLP_H} PUBLIC_HEADER DESTINATION lib/wc3lib/blp)