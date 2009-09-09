set(UIS
    widget.ui
)

qt_wrap_ui(vjassdoc ui_widget.hpp
             ui_widget.cpp ${UIS})
#QT4_WRAP_UI(UIS_HEADERS ${UIS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})