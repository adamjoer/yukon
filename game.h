#pragma once

#include "linked_list.h"
#include "io.h"

extern bool play_phase_active;
extern bool show_columns;

void game_loop();

static void execute_user_command(enum command command);

static void load_default_deck();

static void free_columns();

static bool move_card_action();

static bool is_valid_move(node *moved_node, node *destination_node, bool is_to_foundation);

static void quit_game();

static void distribute_cards_into_columns_for_game(linked_list *list);

static void distribute_cards_into_columns_for_show(linked_list *list, bool visible);
