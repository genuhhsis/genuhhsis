cmake_minimum_required(VERSION 3.27)
project(CS480A1)

set(CMAKE_CXX_STANDARD 11)

add_executable(CS480A1 main.cpp
        log.c
        pageTableLevel.cpp
        pageTable.cpp
        Makefile
        tracereader.c)
