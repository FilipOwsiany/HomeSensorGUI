cmake_minimum_required(VERSION 3.10)
project(logger CXX)

add_library(logger INTERFACE)

target_include_directories(logger
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)