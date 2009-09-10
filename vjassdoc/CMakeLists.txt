cmake_minimum_required(VERSION 2.4.7)
cmake_policy(SET CMP0005 NEW)

project(vjassdoc)

find_package(Gettext REQUIRED)
find_package(Boost REQUIRED)

# setup flags

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	add_definitions(-DDEBUG=1)
	set(USE_DEBUG 1)
	message(STATUS "Build type is \"Debug\", using -DDEBUG.")
endif (CMAKE_BUILD_TYPE STREQUAL "Debug")

find_package(Qt4)
find_package(KDE4) #FindGettext

if (QT4_FOUND AND KDE4_FOUND)
	add_definitions(-DGUI=1)
	set(USE_GUI 1)
	message(STATUS "Qt 4 and KDE 4 have been found, using -DGUI.")
	include(${QT_USE_FILE})
	add_definitions(${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
	#add_definitions(-DQT_STL)
	link_directories(${KDE4_LIB_DIR})
	include_directories(${KDE4_INCLUDE_DIR} ${QT_INCLUDE_DIR})
endif (QT4_FOUND AND KDE4_FOUND)

find_library(SQLITE_FOUND sqlite3)

if (SQLITE_FOUND)
	add_definitions(-DSQLITE=1)
	set(USE_SQLITE 1)
	message(STATUS "sqlite3 has been found, using -DSQLITE.")
endif (SQLITE_FOUND)

#add_definitions(-DSQLITE=1 -DDEBUG=1 -DGUI=1) # test

if (UNIX)
	add_definitions(-DUNIX=1)
elseif (WIN32)
	add_definitions(-DWIN32=1)
else (UNIX)
	message("Warning: Unknown operating system.")
endif (UNIX)

add_definitions(-DLOCALE_DIR=\\"${CMAKE_INSTALL_PREFIX}/share/local\\")

include_directories(src)
add_subdirectory(src)
add_subdirectory(locale)

if (USE_DEBUG)
	add_subdirectory(tests)
endif (USE_DEBUG)
