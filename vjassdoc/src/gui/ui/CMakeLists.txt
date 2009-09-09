SET(UIS
    widget.ui
)

QT4_WRAP_UI(UIS_HEADERS ${UIS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})