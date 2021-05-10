#ifndef CARD_GAME_GAME_H
#define CARD_GAME_GAME_H

#include "linked_list.h"

void game_loop();

void execute_user_command(int command);

void move_card_action();

bool is_valid_move(node *moved_node, node *destination_node, bool is_to_foundation);

void quit_game();

void free_columns();

linked_list **distribute_cards_into_columns_for_game(linked_list *list_copy);

linked_list **distribute_cards_into_columns_for_show(linked_list *list_copy, bool visible);

int get_card_value(char name);

char get_card_name(int value);

#endif //CARD_GAME_GAME_H
