set(MOC_H
	widget.hpp
)

set(RESOURCES
)

set(SRC
	mainwindow.cpp
	widget.cpp
	main.cpp
)

add_subdirectory(ui)

#qt4_automoc(${SRC})
#execute_process(COMMAND moc ${CMAKE_CURRENT_SOURCE_DIR}/widget.hpp -o ${CMAKE_CURRENT_SOURCE_DIR}/moc_widget.cpp)

#QT4_WRAP_CPP(MOC_SRC ${MOC_H})
#QT4_ADD_RESOURCES(RESOURCES ${RESOURCES})
QT4_AUTOMOC(widget.cpp)

find_library(vjassdoclib REQUIRED)
kde4_add_executable(vjassdocgui ${SRC})
target_link_libraries(vjassdocgui vjassdoclib ${QT_LIBRARIES} ${KDE4_KIO_LIBS} ${KDE4_KUTILS_LIBS} ${KDE4_KPARTS_LIBS})

#install

install(TARGETS
	vjassdocgui
	DESTINATION bin
)
