cmake_minimum_required(VERSION 4.1)
project(Excercises)

set(CMAKE_CXX_STANDARD 23)

add_executable(Excercises main.cpp
        Lab1/lab1.cpp
        Lab1/lab1.h
        gtest_lite.h)
