cmake_minimum_required(VERSION 2.4.7)
cmake_policy(SET CMP0005 NEW)

project(vjasstrans)

if (UNIX)
	set(EXECUTABLE_NAME
	vjasstrans
	)
	set(EXECUTABLE_INSTALL_DIR
	/usr/bin
	)
	set(LOCALE_INSTALL_DIR
	/usr/share/locale
	)
	add_definitions(-DUNIX=1)
elseif (WIN32)
	set(EXECUTABLE_NAME
	vjasstrans.exe
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

add_definitions(-DLOCALE_DIR=\"${LOCALE_INSTALL_DIR}\")

include_directories(src)
add_subdirectory(src)
#add_subdirectory(locale)
