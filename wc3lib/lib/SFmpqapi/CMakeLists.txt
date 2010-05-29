WC3LIB_ENABLE_EXCEPTIONS()

WC3LIB_GET_H(wc3lib_LIB_SFMPQAPI_H)
WC3LIB_GET_SRC(wc3lib_LIB_SFMPQAPI_SRC)

add_library(sfmpq SHARED SFmpqapi.cpp)
add_subdirectory(linux)

# install

install(TARGETS sfmpq DESTINATION lib/wc3lib/lib/)
install(FILES ${wc3lib_LIB_SFMPQAPI_H} PUBLIC_HEADER DESTINATION include/wc3lib/lib/SFmpqapi)