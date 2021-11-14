cmake_minimum_required(VERSION 3.16)
project(yukon C)

set(CMAKE_C_STANDARD 99)

add_executable(yukon
    src/main.c
    src/linked_list.c
    src/gui.c
    src/io.c
    src/yukon.c
)
