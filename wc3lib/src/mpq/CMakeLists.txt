WC3LIB_ENABLE_EXCEPTIONS()

set(wc3lib_MPQ_SRC
	block.cpp
	hash.cpp
	mpq.cpp
	mpqfile.cpp
	sector.cpp
)

set(wc3lib_MPQ_H
	algorithm.hpp
	block.hpp
	hash.hpp
	mpqfile.hpp
	mpq.hpp
	platform.hpp
	sector.hpp
)

add_library(wc3libmpq SHARED ${wc3lib_MPQ_SRC})
target_link_libraries(wc3libmpq wc3libcore ${GETTEXT_LIBRARIES} ${Boost_LIBRARIES})

#install

install(TARGETS wc3libmpq DESTINATION lib/wc3lib/)
install(FILES ${wc3lib_MPQ_H} PUBLIC_HEADER DESTINATION include/wc3lib/mpq)
