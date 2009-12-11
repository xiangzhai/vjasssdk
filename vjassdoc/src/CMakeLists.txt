set(VJASSDOC_SRC
	#objects
	object.cpp
	comment.cpp
	keyword.cpp
	key.cpp
	textmacro.cpp
	textmacroinstance.cpp
	externalcall.cpp
	type.cpp
	global.cpp
	local.cpp
	member.cpp
	parameter.cpp
	functioninterface.cpp
	function.cpp
	method.cpp
	call.cpp
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

set(VJASSDOC_H
	object.hpp
	comment.hpp
	keyword.hpp
	key.hpp
	textmacro.hpp
	textmacroinstance.hpp
	externalcall.hpp
	type.hpp
	local.hpp
	global.hpp
	member.hpp
	functioninterface.hpp
	function.hpp
	method.hpp
	call.hpp
	parameter.hpp
	implementation.hpp
	hook.hpp
	interface.hpp
	struct.hpp
	module.hpp
	scope.hpp
	library.hpp
	sourcefile.hpp
	doccomment.hpp
	syntaxerror.hpp
	file.hpp
	parser.hpp
	compiler.hpp
	vjassdoc.hpp
	utilities.hpp
)

execute_process(COMMAND flex vjass.l WORKING_DIRECTORY src)
execute_process(COMMAND bison -d vjass.y WORKING_DIRECTORY src)

set(VJASSDOC_LANG_SRC
	vjass.yy.c
	vjass.tab.c
)

# library
add_library(vjassdoclib ${VJASSDOC_LANG_SRC} ${VJASSDOC_SRC})
target_link_libraries(vjassdoclib ${GETTEXT_LIBRARIES} ${Boost_LIBRARIES})
if (${FLAGS} MATCHES "SQLITE")
	target_link_libraries(vjassdoclib sqlite3) #${SQLITE_LIBRARIES}
endif (${FLAGS} MATCHES "SQLITE")

# program
add_executable(vjassdoc main.cpp)
target_link_libraries(vjassdoc vjassdoclib)

if (${FLAGS} MATCHES "KDE")
	add_subdirectory(gui)
endif (${FLAGS} MATCHES "KDE")

#install

install(FILES
	${VJASSDOC_H}
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
