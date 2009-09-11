cmake_minimum_required(VERSION 2.6.4)

project(vjasstrans)

find_package(Gettext REQUIRED)
find_package(Boost REQUIRED)
# todo: require boost tokenizer component only.

if (UNIX)
	add_definitions(-DUNIX=1)
elseif (WIN32)
	add_definitions(-DWIN32=1)
else (UNIX)
	message("Warning: Unknown operating system.")
endif (UNIX)

add_definitions(-DLOCALE_DIR=\"${CMAKE_INSTALL_PREFIX}/share/local\")

include_directories(src)
add_subdirectory(src)
#add_subdirectory(locale)
