set(LIB_SRC
	#objects
	object.cpp
	comment.cpp
	keyword.cpp
	textmacro.cpp
	textmacroinstance.cpp
	type.cpp
	global.cpp
	local.cpp
	member.cpp
	parameter.cpp
	functioninterface.cpp
	function.cpp
	method.cpp
	implementation.cpp
	interface.cpp
	struct.cpp
	module.cpp
	scope.cpp
	library.cpp
	sourcefile.cpp
	doccomment.cpp
	#program
	file.cpp
	parser.cpp
	vjassdoc.cpp
)

set(HEADERS
	object.h
	comment.h
	keyword.h
	textmacro.h
	textmacroinstance.h
	type.h
	local.h
	global.h
	member.h
	functioninterface.h
	function.h
	method.h
	paramter.h
	implementation.h
	interface.h
	struct.h
	module.h
	scope.h
	library.h
	sourcefile.h
	doccomment.h
	file.h
	parser.h
	vjassdoc.h
)

add_library(vjassdoclib SHARED ${LIB_SRC})
target_link_libraries(vjassdoclib sqlite3)
target_link_libraries(vjassdoclib gettextlib)
add_executable(vjassdoc ${LIB_SRC} main.cpp)
target_link_libraries(vjassdoc sqlite3)
target_link_libraries(vjassdoc gettextlib)

#install

install(FILES
	${HEADERS}
	DESTINATION ${INCLUDE_INSTALL_DIR}
)

install(FILES
	${LIB_NAME}
	DESTINATION ${LIB_INSTALL_DIR}
)

install(FILES
	${EXECUTABLE_NAME}
	DESTINATION ${EXECUTABLE_INSTALL_DIR} #newer cmake ${RUNTIME_OUTPUT_DIRECTORY} 
)