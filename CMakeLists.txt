cmake_minimum_required(VERSION 3.16)
project(card_game C)

set(CMAKE_C_STANDARD 99)

add_executable(card_game
    src/main.c
    src/linked_list.c
    src/gui.c
    src/io.c
    src/game.c
)
