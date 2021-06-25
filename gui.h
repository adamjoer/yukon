#ifndef CARD_GAME_GUI_H
#define CARD_GAME_GUI_H

#include "linked_list.h"

#define MESSAGE_BUFFER_SIZE 256

#define NUMBER_OF_COLUMNS 7
#define LONGEST_COLUMN_LENGTH 11
#define NUMBER_OF_FOUNDATIONS 4

void print_board(linked_list *columns[], linked_list *foundations[]);

void clear_console();

void set_message(char *new_message);

void set_last_command(char *new_last_command);

#endif //CARD_GAME_GUI_H
