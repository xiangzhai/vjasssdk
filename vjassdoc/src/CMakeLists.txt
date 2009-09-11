set(SRC
	#objects
	object.cpp
	comment.cpp
	keyword.cpp
	key.cpp
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
	hook.cpp
	interface.cpp
	struct.cpp
	module.cpp
	scope.cpp
	library.cpp
	sourcefile.cpp
	doccomment.cpp
	#program
	syntaxerror.cpp
	file.cpp
	parser.cpp
	compiler.cpp
	vjassdoc.cpp
	utilities.cpp
)

set(HEADERS
	object.h
	comment.h
	keyword.h
	key.h
	textmacro.h
	textmacroinstance.h
	type.h
	local.h
	global.h
	member.h
	functioninterface.h
	function.h
	method.h
	parameter.h
	implementation.h
	hook.h
	interface.h
	struct.h
	module.h
	scope.h
	library.h
	sourcefile.h
	doccomment.h
	syntaxerror.h
	file.h
	parser.h
	compiler.h
	vjassdoc.h
	utilities.h
)

# library
add_library(vjassdoclib STATIC ${SRC} ${HEADERS}) #SHARED
if (USE_SQLITE)
	target_link_libraries(vjassdoclib sqlite3)
endif (USE_SQLITE)
target_link_libraries(vjassdoclib gettextlib)
# todo: link with boost?

# program
add_executable(vjassdoc main.cpp)
if (USE_SQLITE)
	target_link_libraries(vjassdoc sqlite3)
endif (USE_SQLITE)
target_link_libraries(vjassdoc gettextlib vjassdoclib)
# todo: link with boost?

if (USE_GUI)
	add_subdirectory(gui)
endif (USE_GUI)

#install

install(FILES
	${HEADERS}
	DESTINATION include/vjassdoc/
)

install(TARGETS
	vjassdoclib
	DESTINATION lib
)

install(TARGETS
	vjassdoc
	DESTINATION bin
)
