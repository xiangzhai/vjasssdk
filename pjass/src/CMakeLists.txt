set(SRC
	grammar.tab.c
	grammar.y
	lex.yy.c
	misc.c
	token.l
)

add_executable(pjass ${SRC})

#install

install(FILES
	pjass
	DESTINATION bin
)