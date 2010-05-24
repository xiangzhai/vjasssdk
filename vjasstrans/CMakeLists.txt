cmake_minimum_required(VERSION 2.6)

project(vjasstrans)

find_package(Gettext REQUIRED)

if (GETTEXT_FOUND)
	message(STATUS "Gettext has been found.")
	include_directories(${GETTEXT_INCLUDE_DIR})
else (GETTEXT_FOUND)
	message(STATUS "Gettext has not been found.")
	# cancel
endif (GETTEXT_FOUND)

find_package(Boost COMPONENTS filesystem REQUIRED)

if (Boost_FOUND)
	message(STATUS "Boost has been found.")
	include_directories(${Boost_INCLUDE_DIR})
else (Boost_FOUND)
	message(STATUS "Boost has not been found.")
	# cancel
endif (Boost_FOUND)

include_directories(src)
add_subdirectory(src)
#add_subdirectory(locale)
