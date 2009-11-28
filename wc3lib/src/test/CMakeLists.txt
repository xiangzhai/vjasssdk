add_executable(converter converter.cpp)
target_link_libraries(converter wc3lib ${Boost_LIBRARIES})

#install

install(TARGETS converter DESTINATION bin/wc3lib/)
