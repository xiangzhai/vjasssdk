add_executable(blptest blptest.cpp)
target_link_libraries(blptest wc3lib ${Boost_LIBRARIES})
add_executable(mdlxtest mdlxtest.cpp)
target_link_libraries(mdlxtest wc3lib ${Boost_LIBRARIES})