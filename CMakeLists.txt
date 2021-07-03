cmake_minimum_required(VERSION 3.19)
project(card_game C)

set(CMAKE_C_STANDARD 99)

add_executable(card_game
    main.c
    linked_list.c
    gui.c
    io.c
    game.c
)
