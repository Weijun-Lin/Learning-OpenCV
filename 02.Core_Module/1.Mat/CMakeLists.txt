cmake_minimum_required(VERSION 3.5)
project( Mat )

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable( Mat main.cpp )
target_link_libraries( Mat ${OpenCV_LIBS} )