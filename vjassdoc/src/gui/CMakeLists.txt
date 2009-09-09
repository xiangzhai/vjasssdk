set(SRC
	mainwindow.cpp
	main.cpp
)

add_subdirectory(ui)

qt4_automoc(${SRC})
find_library(vjassdoclib REQUIRED)
add_executable(vjassdocgui ${SRC} ${UIS_HEADERS})
target_link_libraries(vjassdocgui vjassdoclib ${QT_LIBRARIES} ${KDE4_KIO_LIBS} ${KDE4_KUTILS_LIBS} ${KDE4_KPARTS_LIBS})

#install

install(TARGETS
	vjassdocgui
	DESTINATION bin
)