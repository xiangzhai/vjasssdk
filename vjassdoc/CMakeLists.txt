cmake_minimum_required(VERSION 2.4.7)
cmake_policy(SET CMP0005 NEW)

project(vjassdoc)

find_library(gettext REQUIRED)

add_definitions(-DSQLITE) #test, remove if you want to 
add_definitions(-DDEBUG) #test, remove if you want to 

if (SQLITE)
	find_library(sqlite3 REQUIRED)
endif (SQLITE)

if (UNIX)
	set(LIB_NAME
	libvjassdoclib.so
	)
	set(EXECUTABLE_NAME
	vjassdoc
	)
	set(INCLUDE_INSTALL_DIR
	/usr/include/vjassdoc
	)
	set(LIB_INSTALL_DIR
	/usr/lib
	)
	set(EXECUTABLE_INSTALL_DIR
	/usr/bin
	)
	set(LOCALE_INSTALL_DIR
	/usr/share/locale
	)
	set(KATEXMLDIR
	/usr/kde/4.2/share/apps/katepart/syntax
	)
	add_definitions(-DUNIX=1)
elseif (WIN32)
	set(LIB_NAME
	vjassdoc.dll
	)
	set(EXECUTABLE_NAME
	vjassdoc.exe
	)
	set(INCLUDE_INSTALL_DIR
	./include
	)
	set(LIB_INSTALL_DIR
	./lib
	)
	set(EXECUTABLE_INSTALL_DIR
	.
	)
	set(LOCALE_INSTALL_DIR
	./locale
	)
	set(KATEXMLDIR
	.
	)
	add_definitions(-DWIN32=1)
else (UNIX)
	message("Warning: Unknown operating system.")
endif (UNIX)

add_definitions(-DLOCALE_DIR=\"${LOCALE_INSTALL_DIR}\")

include_directories(src)
add_subdirectory(src)
add_subdirectory(locale)

#if (DEBUG)
	add_subdirectory(tests)
#endif (DEBUG)
