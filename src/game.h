#pragma once

#include "linked_list.h"
#include "io.h"

static void game_init();

static void game_destroy();

void start_game();

static void game_loop();

static void execute_user_command(enum Command command);

static void load_default_deck();

static void shuffle_split();

static bool move_card();

static bool is_valid_move(Node *moving_node, Node *destination_node, bool from_foundation, bool to_foundation);

static void empty_columns();

static void quit_game();
