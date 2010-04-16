set(wc3lib_LIB_READBLEND_SRC                            
	readblend.c
)

set(wc3lib_LIB_READBLEND_H
	abs-file.h
	blendtype.h
	readblend.h
)

add_library(readblend SHARED ${wc3lib_LIB_READBLEND_SRC}) 

install(TARGETS readblend DESTINATION lib/wc3lib/lib/)
install(FILES ${wc3lib_LIB_READBLEND_H} PUBLIC_HEADER DESTINATION include/wc3lib/lib)