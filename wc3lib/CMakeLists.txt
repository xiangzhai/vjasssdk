cmake_minimum_required(VERSION 2.6)

project(wc3lib)

#if (CMAKE_BUILD_TYPE STREQUAL "Debug")
#	add_definitions(-DDEBUG=1)
#	set(USE_DEBUG 1)
#	message(STATUS "Build type is \"Debug\", using -DDEBUG.")
#endif (CMAKE_BUILD_TYPE STREQUAL "Debug")

find_package(Gettext REQUIRED)

if (GETTEXT_FOUND)
	message(STATUS "Gettext has been found.")
	include_directories(${GETTEXT_INCLUDE_DIR})
endif (GETTEXT_FOUND)

include_directories(src lib)
add_definitions(-DLOCALE_DIR=\"${CMAKE_INSTALL_PREFIX}/share/locale\")
#add_definitions(-DJPEG -DTGA -DPNG -DBLEND -DMAX) # test

#add_subdirectory(doc)
#add_subdirectory(ui)
add_subdirectory(src)
#add_subdirectory(icons)
