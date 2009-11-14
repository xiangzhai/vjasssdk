set(wc3lib_MDLX_READBLEND_SRC                            
	readblend.c
)

set(wc3lib_MDLX_READBLEND_H
	abs-file.h
	blendtype.h
	readblend.h
)

add_library(wc3libmdlxreadblend SHARED ${wc3lib_MDLX_READBLEND_SRC}) 

install(TARGETS wc3libmdlxreadblend DESTINATION lib/wc3lib/mdlx/readblend/)
install(FILES ${wc3lib_MDLX_READBLEND_H} PUBLIC_HEADER DESTINATION lib/wc3lib/mdlx/readblend)