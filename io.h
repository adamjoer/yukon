#ifndef CARD_GAME_IO_H
#define CARD_GAME_IO_H

#define IN_BUFFER_SIZE 64

#define FILE_NOT_FOUND 1
#define INVALID_FORMAT 2
#define INVALID_RANK 3
#define INVALID_SUIT 4
#define INVALID_SUIT_CARD_COUNT 5
#define INVALID_RANK_CARD_COUNT 6

#define INVALID_INPUT_FORMAT 1
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

extern char moved_card[];
extern char source_column[];
extern char destination_column[];
extern char argument[];

linked_list *load_from_file(char *filepath, bool check_file);

int validate_file(char *filepath);

int get_user_command();

bool is_valid_column(char *string);

bool is_valid_foundation(char *string);

bool is_valid_card(char *string);

void save_deck_to_file(linked_list *list, char *filepath);

#endif //CARD_GAME_IO_H
