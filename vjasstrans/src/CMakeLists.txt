set(SRC
	main.cpp
	parser.cpp
	options.cpp
)

add_executable(vjasstrans ${SRC})
target_link_libraries(vjasstrans ${GETTEXT_LIBRARIES} ${Boost_LIBRARIES})

#install

install(TARGETS
	vjasstrans
	DESTINATION bin
)