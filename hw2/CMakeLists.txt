cmake_minimum_required(VERSION 3.8)
set(CMAKE_CXX_STANDARD 11)
project(hw2)

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

set(SOURCE_FILES main.cpp)
add_executable(hw2 ${SOURCE_FILES})
target_link_libraries (hw2 ${OpenCV_LIBS})

