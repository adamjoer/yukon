#ifndef CARD_GAME_IO_H
#define CARD_GAME_IO_H

#define FILE_NOT_FOUND 1
#define UNKNOWN_FORMAT 2
#define UNKNOWN_NAME 3
#define UNKNOWN_SUIT 4
#define WRONG_SUIT_CARD_COUNT 5
#define WRONG_NAME_CARD_COUNT 6

#define WRONG_INPUT_FORMAT 1
#define PLAY 2
#define QUIT_GAME 3
#define QUIT_PROGRAM 4
#define LOAD_FILE 5
#define SHOW_CARDS 6
#define SHUFFLE_SPLIT 7
#define SHUFFLE_RANDOM 8
#define SAVE_DECK 9
#define MOVE_CARD 10

#include "linked_list.h"

linked_list *load_from_file(char *filepath);

int validate_file(char *filepath);

int get_user_command();

bool is_valid_column(char *string);

bool is_valid_card(char *string);

void save_deck_to_file(linked_list *list, char *filepath);

char *get_command();

char *get_moved_card();

char *get_source_column();

char *get_destination_column();

char *get_argument();

#endif //CARD_GAME_IO_H
