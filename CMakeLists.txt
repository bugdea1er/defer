cmake_minimum_required(VERSION 3.12)
project(defer LANGUAGES CXX)

add_subdirectory(include)

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    enable_testing()
    add_subdirectory(tests)
endif()
