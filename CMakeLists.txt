cmake_minimum_required(VERSION 3.30)
project(library_system2 C)

set(CMAKE_C_STANDARD 17)

add_executable(library_system2 main.c
        librarySystemFunctions.c
        librarySystemFunctions.h)
