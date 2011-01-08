WC3LIB_ENABLE_EXCEPTIONS()

WC3LIB_GET_H(wc3lib_LIB_SFMPQAPI_H)
WC3LIB_GET_SRC(wc3lib_LIB_SFMPQAPI_SRC)

# UNIX/Linux support
add_subdirectory(linux)
if (UNIX)
	message(STATUS "SFmpqapi: Is UNIX.")
	set(wc3lib_LIB_SFMPQAPI_SRC ${wc3lib_LIB_SFMPQAPI_SRC} ${wc3lib_LIB_SFMPQAPI_LINUX_SRC})
endif ()

# remove static library files (see TODO file)
list(REMOVE_ITEM wc3lib_LIB_SFMPQAPI_H ${CMAKE_CURRENT_SOURCE_DIR}/SFmpqapi_no-lib.h ${CMAKE_CURRENT_SOURCE_DIR}/SFmpq_static.h)
list(REMOVE_ITEM wc3lib_LIB_SFMPQAPI_SRC ${CMAKE_CURRENT_SOURCE_DIR}/SFmpqapi_no-lib.cpp ${CMAKE_CURRENT_SOURCE_DIR}/SFmpq_static.cpp)

add_library(sfmpq SHARED ${wc3lib_LIB_SFMPQAPI_SRC})

# install

install(TARGETS sfmpq DESTINATION lib/wc3lib/lib/)
install(FILES ${wc3lib_LIB_SFMPQAPI_H} PUBLIC_HEADER DESTINATION include/wc3lib/lib/SFmpqapi)