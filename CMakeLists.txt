cmake_minimum_required(VERSION 3.5)

project(SortTest LANGUAGES C CXX)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK REQUIRED gtk+-3.0)

include_directories(${GTK_INCLUDE_DIRS})
link_directories(${GTK_LIBRARY_DIRS})

add_executable(SortTest sortUI.c)

add_library(SortDisplay 
    SortDisplay.h
    SortDisplay.c)

target_include_directories(SortDisplay PUBLIC ${GTK_INCLUDE_DIRS})

target_link_libraries(SortTest
    SortDisplay
    ${GTK_LIBRARIES}
    -lm)