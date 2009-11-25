add_executable(converter converter.cpp)
target_link_libraries(converter wc3lib ${Boost_LIBRARIES})