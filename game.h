#pragma once

#include "linked_list.h"
#include "io.h"

void game_loop();

static void execute_user_command(enum command command);

static void load_default_deck();

static void free_columns();

static bool move_card_action();

static bool is_valid_move(node *moving_node, node *destination_node, bool is_to_foundation);

static void quit_game();

static void generate_columns_game();

static void generate_columns_show(bool visible);
