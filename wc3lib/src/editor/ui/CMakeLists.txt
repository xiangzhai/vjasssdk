WC3LIB_ENABLE_EXCEPTIONS()

WC3LIB_GET_UI(wc3lib_EDITOR_UI)
kde4_add_ui_files(wc3lib_EDITOR_UI_SRCS ${wc3lib_EDITOR_UI})

# install
install(FILES ${wc3lib_EDITOR_UI_H} PUBLIC_HEADER DESTINATION include/wc3lib/editor)