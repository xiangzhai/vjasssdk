set(SRC
	mainwindow.cpp
	main.cpp
)

add_subdirectory(ui)

qt4_automoc(${SRC})
add_executable(vjassdocgui ${SRC} ${UIS_HEADERS})
#find_library(vjassdoclib REQUIRED)
target_link_libraries(vjassdocgui vjassdoclib ${QT_LIBRARIES} ${KDE4_KIO_LIBS} ${KDE4_KUTILS_LIBS} ${KDE4_KPARTS_LIBS})

#install

install(TARGETS
	vjassdocgui
	DESTINATION bin
)