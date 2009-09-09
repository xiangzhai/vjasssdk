cmake_minimum_required(VERSION 2.4.7)
cmake_policy(SET CMP0005 NEW)

project(vjassdoc)

add_definitions(-DSQLITE=1 -DDEBUG=1 -DGUI=1) # test

if (UNIX)
	add_definitions(-DUNIX=1)
elseif (WIN32)
	add_definitions(-DWIN32=1)
else (UNIX)
	message("Warning: Unknown operating system.")
endif (UNIX)

add_definitions(-DLOCALE_DIR=\"${LOCALE_INSTALL_DIR}\")

#find_package(Gettext REQUIRED)

#if (-DSQLITE)
	#find_package(Sqlite REQUIRED)
#endif (-DSQLITE)

if (-DGUI)
	find_package(Qt4 REQUIRED)
	find_package(KDE4 REQUIRED)
	include(${QT_USE_FILE})
	add_definitions(${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
	#add_definitions(-DQT_STL)
	link_directories(${KDE4_LIB_DIR})
	include_directories(${KDE4_INCLUDE_DIR} ${QT_INCLUDE_DIR})
endif (-DGUI)

include_directories(src)
add_subdirectory(src)
add_subdirectory(locale)

#if (-DDEBUG)
	add_subdirectory(tests)
#endif (-DDEBUG)
