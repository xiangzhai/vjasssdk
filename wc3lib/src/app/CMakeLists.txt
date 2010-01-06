add_executable(converter converter.cpp)
target_link_libraries(converter wc3lib ${Boost_LIBRARIES})

find_package(Qt4 REQUIRED)
find_package(KDE4 REQUIRED)
include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
link_directories(${KDE4_LIB_DIR})
include_directories(${KDE4_INCLUDE_DIR} ${QT_INCLUDE_DIR})
kde4_add_executable(editor editor.cpp)
target_link_libraries(editor wc3lib ${KDE4_KIO_LIBS} ${KDE4_KUTILS_LIBS} ${KDE4_KPARTS_LIBS})

add_executable(jassc jassc.cpp)
target_link_libraries(jassc wc3lib ${Boost_LIBRARIES})

message("Boost libraries ${Boost_LIBRARIES}") # test

#install

install(TARGETS converter jassc DESTINATION bin/wc3lib)
