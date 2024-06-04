cmake_minimum_required(VERSION 3.22.1)

project("native-lib")

add_library(${CMAKE_PROJECT_NAME} SHARED
        Matrix.cpp
        Batch.cpp
        native-lib.cpp)

target_link_libraries(${CMAKE_PROJECT_NAME}
        android
        log
        GLESv2)