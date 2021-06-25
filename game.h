#ifndef CARD_GAME_GAME_H
#define CARD_GAME_GAME_H

#include "linked_list.h"

extern bool play_phase_active;
extern bool show_columns;

void game_loop();

void execute_user_command(int command);

bool move_card_action();

bool is_valid_move(node *moved_node, node *destination_node, bool is_to_foundation);

void quit_game();

void free_columns();

void distribute_cards_into_columns_for_game(linked_list *list);

void distribute_cards_into_columns_for_show(linked_list *list, bool visible);

int get_card_value(char rank);

#endif //CARD_GAME_GAME_H
