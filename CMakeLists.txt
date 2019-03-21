cmake_minimum_required(VERSION 3.8)
project(BitmapLion)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp bmp.cpp bmp.h BitmapInformation.h)
add_executable(BitmapLion ${SOURCE_FILES})