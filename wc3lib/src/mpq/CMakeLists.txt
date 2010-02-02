set(wc3lib_MPQ_SRC
	mpq.cpp
	mpqfile.cpp
)

set(wc3lib_MPQ_H
	algorithm.hpp
	mpqfile.hpp
	mpq.hpp
	platform.hpp
)

add_library(wc3libmpq SHARED ${wc3lib_MPQ_SRC})
target_link_libraries(wc3libmpq ${GETTEXT_LIBRARIES} ${Boost_LIBRARIES})

#install

install(TARGETS wc3libmpq DESTINATION lib/wc3lib/)
install(FILES ${wc3lib_MPQ_H} PUBLIC_HEADER DESTINATION include/wc3lib/mpq)
