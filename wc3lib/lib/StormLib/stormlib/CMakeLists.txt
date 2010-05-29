WC3LIB_ENABLE_EXCEPTIONS()

WC3LIB_GET_H(wc3lib_LIB_STORMLIB_H)
WC3LIB_GET_SRC(wc3lib_LIB_STORMLIB_SRC)

if (APPLE)
	list(REMOVE wc3lib_LIB_STORMLIB_SRC StormPortLinux.cpp)
elseif (UNIX)
	list(REMOVE wc3lib_LIB_STORMLIB_SRC StormPortMac.cpp)
elseif (WIN32)
	list(REMOVE wc3lib_LIB_STORMLIB_SRC StormPortMac.cpp)
	list(REMOVE wc3lib_LIB_STORMLIB_SRC StormPortLinux.cpp)
else ()
	message(FATAL_ERROR "StormLib: Unsupported platform. Supported platforms are Linux/UNIX, Mac OS and Windows.")
endif ()

add_library(qogre SHARED ${wc3lib_LIB_STORMLIB_SRC})
target_link_libraries(qogre OgreMain ${QT_LIBRARIES})

# install
install(TARGETS qogre DESTINATION lib/wc3lib/lib/)
install(FILES ${wc3lib_LIB_QOGRE_H} PUBLIC_HEADER DESTINATION include/wc3lib/lib/qogre) 
