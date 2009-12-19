add_executable(converter converter.cpp)
target_link_libraries(converter wc3lib ${Boost_LIBRARIES})

add_executable(jassc jassc.cpp)
target_link_libraries(jassc wc3lib ${Boost_LIBRARIES})

message("Boost libraries ${Boost_LIBRARIES}") # test

#install

install(TARGETS converter jassc DESTINATION bin/wc3lib)
