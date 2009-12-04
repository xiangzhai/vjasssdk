cmake_minimum_required(VERSION 2.6)

project(vjassdoc)

get_directory_property(FLAGS COMPILE_DEFINITIONS)
message(STATUS "Compile definitions are ${FLAGS}")

find_package(Gettext REQUIRED)

if (GETTEXT_FOUND)
	message(STATUS "Gettext has been found.")
	include_directories(${GETTEXT_INCLUDE_DIR})
else (GETTEXT_FOUND)
	message(STATUS "Gettext has not been found.")
	# cancel
endif (GETTEXT_FOUND)

find_package(Boost REQUIRED)

if (Boost_FOUND)
	message(STATUS "Boost has been found.")
	include_directories(${Boost_INCLUDE_DIRS})
else (Boost_FOUND)
	message(STATUS "Boost has not been found.")
	# cancel
endif (Boost_FOUND)

if (${FLAGS} MATCHES "KDE")
	find_package(Qt4 REQUIRED)
	find_package(KDE4 REQUIRED)
	
	if (QT4_FOUND AND KDE4_FOUND)
		add_definitions(-DGUI=1)
		set(USE_GUI 1)
		message(STATUS "Qt 4 and KDE 4 have been found, using -DGUI.")
		include(${QT_USE_FILE})
		add_definitions(${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
		#add_definitions(-DQT_STL)
		link_directories(${KDE4_LIB_DIR})
		include_directories(${KDE4_INCLUDE_DIR} ${QT_INCLUDE_DIR})
	else (QT4_FOUND AND KDE4_FOUND)
		message(STATUS "Qt 4 or KDE 4 has not been found.")
		# cancel
	endif (QT4_FOUND AND KDE4_FOUND)
endif (${FLAGS} MATCHES "KDE")

if (${FLAGS} MATCHES "SQLITE")
	find_library(sqlite3 REQUIRED)
endif (${FLAGS} MATCHES "SQLITE")

#if (SQLITE_FOUND)
#	add_definitions(-DSQLITE=1)
#	set(USE_SQLITE 1)
#	message(STATUS "sqlite has been found, using -DSQLITE.")
#	add_definitions(${SQLITE_DEFINITIONS})
#	include_directories(${SQLITE_INCLUDE_DIR})
#endif (SQLITE_FOUND)

add_definitions(-DLOCALE_DIR=\"${CMAKE_INSTALL_PREFIX}/share/locale\")
add_definitions(-DSQLITE -DHTML) # test

include_directories(src)
add_subdirectory(src)
add_subdirectory(locale)

if (USE_DEBUG)
	add_subdirectory(tests)
endif (USE_DEBUG)
