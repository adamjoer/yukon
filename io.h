#ifndef CARD_GAME_IO_H
#define CARD_GAME_IO_H

#define FILE_NOT_FOUND 1
#define UNKNOWN_FORMAT 2
#define UNKNOWN_NAME 3
#define UNKNOWN_SUIT 4
#define WRONG_SUIT_CARD_COUNT 5
#define WRONG_NAME_CARD_COUNT 6

#include "linked_list.h"

linked_list *load_from_file(char *filepath);

int validate_file(char *filepath);

#endif //CARD_GAME_IO_H
