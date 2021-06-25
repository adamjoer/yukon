#ifndef CARD_GAME_GUI_H
#define CARD_GAME_GUI_H

#include "linked_list.h"
#include "io.h"

#define MESSAGE_BUFFER_SIZE 256

#define NO_COLUMNS 7
#define LONGEST_COLUMN_LENGTH 11
#define NO_FOUNDATIONS 4

extern char last_command[];
extern char message[];

void print_board(linked_list *columns[], linked_list *foundations[]);

void clear_console();

#endif //CARD_GAME_GUI_H
