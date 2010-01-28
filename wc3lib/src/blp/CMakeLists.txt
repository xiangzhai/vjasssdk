set(wc3lib_BLP_SRC
	blp.cpp
)

set(wc3lib_BLP_H
	blp.hpp
	platform.hpp
)

# todo: is only required when using -DJPEG
find_library(OPENJPEG_LIBS openjpeg)

add_library(wc3libblp SHARED ${wc3lib_BLP_SRC})
target_link_libraries(wc3libblp ${GETTEXT_LIBRARIES} ${Boost_LIBRARIES} ${OPENJPEG_LIBS})

#install

install(TARGETS wc3libblp DESTINATION lib/wc3lib/)
install(FILES ${wc3lib_BLP_H} PUBLIC_HEADER DESTINATION include/wc3lib/blp)
