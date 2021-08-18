#pragma once

#include "linked_list.h"
#include "io.h"

void game_loop();

static void execute_user_command(enum command command);

static void load_default_deck();

static void shuffle_split();

static bool move_card();

static bool is_valid_move(node *moving_node, node *destination_node, bool from_foundation, bool to_foundation);

static void empty_columns();

static void quit_game();
