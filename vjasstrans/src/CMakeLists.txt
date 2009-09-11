set(SRC
	main.cpp
	parser.cpp
	options.cpp
)

add_executable(vjasstrans ${SRC})
target_link_libraries(vjasstrans gettextlib)
# todo: link with boost library?

#install

install(TARGETS
	vjasstrans
	DESTINATION bin
)