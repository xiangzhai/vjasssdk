set(SRC
	main.cpp
	parser.cpp
	options.cpp
)

add_executable(vjasstrans ${SRC})
target_link_libraries(vjasstrans gettextlib)

#install

install(FILES
	${EXECUTABLE_NAME}
	DESTINATION ${EXECUTABLE_INSTALL_DIR} #newer cmake ${RUNTIME_OUTPUT_DIRECTORY} 
)