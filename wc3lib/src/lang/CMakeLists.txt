WC3LIB_ENABLE_EXCEPTIONS()

set(wc3lib_LANG_SRC
	compiler.cpp
	doccomment.cpp
	file.cpp
	language.cpp
	object.cpp
	parser.cpp
	sourcefile.cpp
	syntaxerror.cpp
	utilities.cpp
)

set(wc3lib_LANG_H
	compiler.hpp
	doccomment.hpp
	file.hpp
	language.hpp
	object.hpp
	parser.hpp
	sourcefile.hpp
	syntaxerror.hpp
	utilities.hpp
)

add_subdirectory(jass)
add_subdirectory(jasspp)
add_subdirectory(vjass)
add_subdirectory(zinc)

# library
add_library(wc3liblang ${wc3lib_LANG_SRC} ${wc3lib_LANG_JASS_SRC} ${wc3lib_LANG_JASSPP_SRC} ${wc3lib_LANG_VJASS_SRC} ${wc3lib_LANG_ZINC_SRC})
target_link_libraries(wc3liblang ${GETTEXT_LIBRARIES} ${Boost_LIBRARIES})
if (${FLAGS} MATCHES "SQLITE")
	target_link_libraries(wc3liblang sqlite3) #${SQLITE_LIBRARIES}
endif (${FLAGS} MATCHES "SQLITE")

#install

install(TARGETS wc3liblang DESTINATION lib/wc3lib/)
install(FILES ${wc3lib_LANG_H} PUBLIC_HEADER DESTINATION include/wc3lib/lang)