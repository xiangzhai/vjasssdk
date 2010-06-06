project(mpqeditor)

find_package(KDE4 REQUIRED)
include(KDE4Defaults)
include_directories(${KDE4_INCLUDES} ${QT_INCLUDES} ui)
add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})

#add_subdirectory(doc)
add_subdirectory(ui)
add_subdirectory(src)
#add_subdirectory(icons)

install(FILES mpqeditor.desktop DESTINATION ${SERVICES_INSTALL_DIR})