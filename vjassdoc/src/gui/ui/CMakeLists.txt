set(UI
	widget.ui
)


execute_process(COMMAND uic ${CMAKE_CURRENT_SOURCE_DIR}/widget.ui -o ${CMAKE_CURRENT_SOURCE_DIR}/ui_widget.hpp)

#qt_wrap_ui(vjassdocgui_ui_lib ui_widget.hpp ui_widget.cpp ${UIS})
#QT4_WRAP_UI(UI_H ${UI})
#include_directories(${CMAKE_CURRENT_BINARY_DIR})