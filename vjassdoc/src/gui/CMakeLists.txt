set(VJASSDOC_GUI_MOC_H
	widget.hpp
)

set(VJASSDOC_GUI_RESOURCES
)

set(VJASSDOC_GUI_SRC
	application.cpp
	mainwindow.cpp
	widget.cpp
	main.cpp
)

add_subdirectory(ui)
kde4_add_ui_files(VJASSDOC_GUI_SRC widget.ui)

#qt4_automoc(${VJASSDOC_GUI_SRC})
#execute_process(COMMAND moc ${CMAKE_CURRENT_SOURCE_DIR}/widget.hpp -o ${CMAKE_CURRENT_SOURCE_DIR}/moc_widget.cpp)

#QT4_WRAP_CPP(VJASSDOC_GUI_MOC_SRC ${VJASSDOC_GUI_MOC_H})
#QT4_ADD_RESOURCES(RESOURCES ${VJASSDOC_GUI_RESOURCES})
QT4_AUTOMOC(widget.cpp)

kde4_add_executable(vjassdocgui ${VJASSDOC_GUI_SRC})
target_link_libraries(vjassdocgui vjassdoclib ${QT_LIBRARIES} ${KDE4_KIO_LIBS} ${KDE4_KUTILS_LIBS} ${KDE4_KPARTS_LIBS})

#install

install(TARGETS
	vjassdocgui
	DESTINATION bin
)
