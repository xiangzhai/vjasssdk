set(wc3lib_LANG_VJASS_SRC
	externalcall.cpp
	functioninterface.cpp
	hook.cpp
	implementation.cpp
	interface.cpp
	key.cpp
	keyword.cpp
	library.cpp
	member.cpp
	method.cpp
	module.cpp
	scope.cpp
	struct.cpp
	textmacro.cpp
	textmacroinstance.cpp
)

set(wc3lib_LANG_VJASS_H
	externalcall.hpp
	functioninterface.hpp
	hook.hpp
	implementation.hpp
	interface.hpp
	key.hpp
	keyword.hpp
	library.hpp
	member.hpp
	method.hpp
	module.hpp
	scope.hpp
	struct.hpp
	textmacro.hpp
	textmacroinstance.hpp
)

execute_process(COMMAND flex vjass.l WORKING_DIRECTORY src/lang/vjass)
execute_process(COMMAND bison -d vjass.y WORKING_DIRECTORY src/lang/vjass)

#install

install(FILES ${wc3lib_LANG_VJASS_H} PUBLIC_HEADER DESTINATION include/wc3lib/lang/vjass)