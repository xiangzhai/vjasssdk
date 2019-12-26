include_directories(${CMAKE_CURRENT_SOURCE_DIR}/StormLib)

set (mpqeditor_SRCS
	main.cpp
	mainwindow.cpp
	archivecreationdialog.cpp
	archiveswidget.cpp
)

kde4_add_executable(mpqeditor ${mpqeditor_SRCS})

target_link_libraries(mpqeditor -lStormLib ${KDE4_KIO_LIBS} ${KDE4_KUTILS_LIBS} ${KDE4_KPARTS_LIBS})

install(TARGETS mpqeditor ${INSTALL_TARGETS_DEFAULT_ARGS})
