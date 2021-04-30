#ifndef CARD_GAME_GUI_H
#define CARD_GAME_GUI_H

#include "linked_list.h"

void print_board(linked_list *columns[]);

void clear_console();

void set_message(char *new_message);

void set_last_command(char *new_last_command);

#endif //CARD_GAME_GUI_H
