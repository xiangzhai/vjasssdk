set(wc3lib_LANG_JASS_SRC
	call.cpp
	comment.cpp
	function.cpp
	global.cpp
	jass.cpp
	local.cpp
	parameter.cpp
	type.cpp
)

set(wc3lib_LANG_JASS_H
	call.hpp
	comment.hpp
	function.hpp
	global.hpp
	jass.hpp
	local.hpp
	parameter.hpp
	type.hpp
)

#install

install(FILES ${wc3lib_LANG_JASS_H} PUBLIC_HEADER DESTINATION include/wc3lib/lang/jass)