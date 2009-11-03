cmake_minimum_required(VERSION 2.6.4)

project(wc3lib)

# setup flags

#if (CMAKE_BUILD_TYPE STREQUAL "Debug")
#	add_definitions(-DDEBUG=1)
#	set(USE_DEBUG 1)
#	message(STATUS "Build type is \"Debug\", using -DDEBUG.")
#endif (CMAKE_BUILD_TYPE STREQUAL "Debug")

find_package(Gettext REQUIRED)
# todo: require tokenizer component only
find_package(Boost REQUIRED)

include_directories(src)
#add_definitions(${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
add_definitions(-DLOCALE_DIR=\"${CMAKE_INSTALL_PREFIX}/share/local\")
add_definitions(-DBLEND) # test

#add_subdirectory(doc)
#add_subdirectory(ui)
add_subdirectory(src)
#add_subdirectory(icons)
