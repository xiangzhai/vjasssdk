cmake_minimum_required(VERSION 2.4.7)

project(vjassdoc)

#set(FOUND_SQLITE
#find_library(sqlite3 REQUIRED)
#)

#if (FOUND_SQLITE)
	add_definitions(-DSQLITE=1)
#endif (FOUND_SQLITE)

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
	add_definitions(-DWIN32=1)
else (UNIX)
	message("Warning: Unknown operating system.")
endif (UNIX)

#add_definitions(-DLOCALE_DIR="\"${LOCALE_INSTALL_DIR}\"")

include_directories(src)
add_subdirectory(src)
add_subdirectory(locale)
