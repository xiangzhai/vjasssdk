cmake_minimum_required(VERSION 2.6.4)

project(wc3lib)

find_package(Gettext REQUIRED)
# todo: require tokenizer component only
find_package(Boost REQUIRED)

include_directories(src)
#add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})

#add_subdirectory(doc)
#add_subdirectory(ui)
add_subdirectory(src)
#add_subdirectory(icons)
