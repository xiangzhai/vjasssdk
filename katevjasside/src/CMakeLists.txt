#In order to use KURLRequester, you need to link 'libkio'. Just add "-lkio" to 
#the target (using the automake manager).


set(katevjasside_SRCS
	plugin.cpp
	pluginview.cpp
	jasshelperpluginconfigpage.cpp)

kde4_add_plugin(katevjasside ${katevjasside_SRCS})
target_link_libraries(katevjasside /usr/kde/4.2/lib/libkio.so.5.2.0 ${KDE4_KDEUI_LIBS} kateinterfaces)

########### install files ###############
install(TARGETS katevjasside DESTINATION ${PLUGIN_INSTALL_DIR})