cmake_minimum_required(VERSION 2.6)

project(pjass)

add_definitions(-DLOCALE_DIR=\"${CMAKE_INSTALL_PREFIX}/share/local\")

include_directories(src)
add_subdirectory(src)
add_subdirectory(locale)