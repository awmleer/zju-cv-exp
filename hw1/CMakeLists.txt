cmake_minimum_required(VERSION 3.8)
set(CMAKE_CXX_STANDARD 11)
project(hw1)

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

set(SOURCE_FILES main.cpp)
add_executable(hw1 ${SOURCE_FILES})
target_link_libraries (hw1 ${OpenCV_LIBS})

