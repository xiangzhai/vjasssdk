set(wc3lib_LIB_QOGRE_SRC                            
	QOgreWidget.cpp
)

set(wc3lib_LIB_QOGRE_H
	ExampleFrameListener.h
	QOgreWidget.h
)

QT4_AUTOMOC(${wc3lib_LIB_QOGRE_SRC})
add_library(qogre SHARED ${wc3lib_LIB_QOGRE_SRC})
target_link_libraries(qogre OgreMain ${QT_LIBRARIES})

install(TARGETS qogre DESTINATION lib/wc3lib/lib/qogre)
install(FILES ${wc3lib_LIB_QOGRE_H} PUBLIC_HEADER DESTINATION include/wc3lib/lib/qogre)