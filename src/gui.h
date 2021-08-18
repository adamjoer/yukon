#pragma once

#include "linked_list.h"

#define MESSAGE_BUFFER_SIZE 256

#define NO_COLUMNS 7
#define LONGEST_COLUMN_LENGTH 11
#define NO_FOUNDATIONS 4

void print_board(linked_list columns[], linked_list foundations[], bool show_columns, bool play_phase_active);

static void clear_console();

void generate_columns_game(linked_list *deck, linked_list columns[]);

void generate_columns_show(linked_list *deck, linked_list columns[], bool visible);

void set_message(char *new_message);

void set_last_command(char *new_last_command);
