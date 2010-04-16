cmake_minimum_required(VERSION 2.8.1)

project(wc3lib)

macro(WC3LIB_ENABLE_EXCEPTIONS)
	if(CMAKE_COMPILER_IS_GNUCC)
		#message("Here it is ${CMAKE_CXX_FLAGS}")
		#string(REPLACE "-fno-exceptions" "-fexceptions" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
		#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" CACHE STRING "CXX flags" FORCE)
		#message("Here it is ${CMAKE_CXX_FLAGS}")
		message("Enabled exceptions.")
	endif(CMAKE_COMPILER_IS_GNUCC)
endmacro()

function(WC3LIB_GET_H resultName)
	file(GLOB list *.hpp)
	list(APPEND result ${list})
	file(GLOB list *.hxx)
	list(APPEND result ${list})
	file(GLOB list *.h)
	list(APPEND result ${list})
	set(${resultName} ${result} PARENT_SCOPE)
endfunction()

function(WC3LIB_GET_SRC resultName)
	file(GLOB list *.cpp)
	list(APPEND result ${list})
	file(GLOB list *.cxx)
	list(APPEND result ${list})
	file(GLOB list *.c)
	list(APPEND result ${list})
	set(${resultName} ${result} PARENT_SCOPE)
endfunction()

function(WC3LIB_GET_COMPILE_FLAGS resultName)
	get_directory_property(FLAGS DIRECTORY . COMPILE_DEFINITIONS)
	set(${resultName} ${FLAGS} PARENT_SCOPE)
endfunction()

function(WC3LIB_SET_COMPILE_FLAG variable flagName)
	get_directory_property(FLAGS DIRECTORY . COMPILE_DEFINITIONS)
	list(FIND FLAGS ${flagName} variable)
	string(COMPARE EQUAL "${variable}" "-1" ISEQUAL)
	if (${ISEQUAL})
		set(variable 0)
	else()
		set(variable 1)
	endif()
	message("Flag \"${flagName}\" is ${variable}.")
endfunction()

# custom compilation flags
WC3LIB_GET_COMPILE_FLAGS(FLAGS)
message("Compile flags are: \"${FLAGS}\".")
WC3LIB_SET_COMPILE_FLAG(FLAG_BLEND_USE "BLEND")
WC3LIB_SET_COMPILE_FLAG(FLAG_EDITOR_USE "EDITOR")

find_package(Gettext REQUIRED) 

set(Boost_USE_MULTITHREADED TRUE)
set(Boost_DEBUG TRUE) # debug mode
set(Boost_DETAILED_FAILURE_MSG TRUE)
set(Boost_ADDITIONAL_VERSIONS "1.78" "1.78.0" "1.79" "1.79.0")
find_package(Boost 1.42.0 COMPONENTS filesystem thread REQUIRED)

if(FLAG_EDITOR_USE)
	find_package(Qt4 REQUIRED)
	find_package(KDE4 REQUIRED)
endif()

#find_library(Ogre REQUIRED) # todo Use custom cmake module to find Ogre.
# todo: is only required when using -DJPEG
find_library(OPENJPEG_LIBS openjpeg) # todo Use custom cmake module to find openjpeg.

#include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
link_directories(${Boost_LIBRARY_DIRS} ${KDE4_LIB_DIR})
include_directories(${GETTEXT_INCLUDE_DIR} ${Boost_INCLUDE_DIRS} ${KDE4_INCLUDE_DIR} ${QT_INCLUDE_DIR} ${Ogre_INCLUDE_DIR})
include_directories(/usr/include/OGRE)

add_definitions(-DLOCALE_DIR=\"${CMAKE_INSTALL_PREFIX}/share/locale\")


include_directories(src lib)

#add_subdirectory(doc)
#add_subdirectory(ui)
add_subdirectory(lib)
add_subdirectory(src)
#add_subdirectory(icons)

include(CPack)

