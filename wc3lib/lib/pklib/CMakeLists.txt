WC3LIB_ENABLE_EXCEPTIONS()

set(wc3lib_LIB_PKLIB_SRC
	crc32.c
	explode.c
	implode.c
)

set(wc3lib_LIB_PKLIB_H
	pklib.h
)

add_library(pklib SHARED ${wc3lib_LIB_PKLIB_SRC} ${Boost_LIBRARIES})

# install

install(TARGETS pklib DESTINATION lib/wc3lib/lib/)
install(FILES ${wc3lib_LIB_PKLIB_H} PUBLIC_HEADER DESTINATION include/wc3lib/lib)
