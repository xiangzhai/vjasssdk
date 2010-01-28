set(wc3lib_SLK_SRC
	slk.cpp
)

set(wc3lib_SLK_H
	slk.hpp
)

add_library(wc3libslk SHARED ${wc3lib_SLK_SRC})
target_link_libraries(wc3libslk ${GETTEXT_LIBRARIES} ${Boost_LIBRARIES})

#install

install(TARGETS wc3libslk DESTINATION lib/wc3lib/)
install(FILES ${wc3lib_SLK_H} PUBLIC_HEADER DESTINATION include/wc3lib/slk)
