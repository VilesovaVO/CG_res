cmake_minimum_required(VERSION 3.31)
project(KG3LABA)

set(CMAKE_CXX_STANDARD 20)

add_executable(KG3LABA main.cpp
        camera.h
        geometry.h
        geometry.cpp
        model.h
        tgaimage.cpp
        tgaimage.h)
