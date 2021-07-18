#pragma once

#include "linked_list.h"
#include "io.h"

extern bool play_phase_active;
extern bool show_columns;

void game_loop();

void execute_user_command(enum command command);

void load_default_deck();

void free_columns();

bool move_card_action();

bool is_valid_move(node *moved_node, node *destination_node, bool is_to_foundation);

void quit_game();

void distribute_cards_into_columns_for_game(linked_list *list);

void distribute_cards_into_columns_for_show(linked_list *list, bool visible);

int get_card_value(char rank);
