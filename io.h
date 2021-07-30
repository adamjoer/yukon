#pragma once

#include "linked_list.h"

#define IN_BUFFER_SIZE 64

enum validation_status {
    OK,
    FILE_NOT_FOUND,
    INVALID_FORMAT,
    INVALID_RANK,
    INVALID_SUIT,
    INVALID_SUIT_CARD_COUNT,
    INVALID_RANK_CARD_COUNT
};

enum command {
    INVALID_INPUT_FORMAT,
    ERROR,
    PLAY,
    QUIT_GAME,
    QUIT_PROGRAM,
    LOAD_FILE,
    SHOW_CARDS,
    SHUFFLE_SPLIT,
    SHUFFLE_RANDOM,
    SAVE_DECK,
    MOVE_CARD
};

extern char moved_card[];
extern char source_column[];
extern char destination_column[];
extern char argument[];

linked_list *load_from_file(char *filepath, bool check_file);

enum validation_status validate_file(char *filepath);

enum command get_user_command();

int get_card_value(char rank);

bool is_valid_column(char *string);

bool is_valid_foundation(char *string);

bool is_valid_card(char *string);

void save_deck_to_file(linked_list *list, char *filepath);
