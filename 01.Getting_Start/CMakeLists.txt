cmake_minimum_required(VERSION 3.5)
project( GettingStart )

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable( GettingStart main.cpp )
target_link_libraries( GettingStart ${OpenCV_LIBS} )