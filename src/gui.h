#pragma once

#include "linked_list.h"

#define MESSAGE_BUFFER_SIZE 256

#define NO_COLUMNS 7
#define LONGEST_COLUMN_LENGTH 11
#define NO_FOUNDATIONS 4

void print_board(LinkedList columns[], LinkedList foundations[]);

void generate_columns_game(LinkedList *deck, LinkedList columns[]);

void generate_columns_show(LinkedList *deck, LinkedList columns[], bool visible);

void set_message(const char *new_message);

void set_last_command(const char *new_last_command);
