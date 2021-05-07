#ifndef CARD_GAME_GAME_H
#define CARD_GAME_GAME_H

#define NUMBER_OF_COLUMNS 7
#define NEXT_SHORTEST_COLUMN_LENGTH 6
#define LONGEST_COLUMN_LENGTH 11
#define NUMBER_OF_FOUNDATIONS 4

#include "linked_list.h"

void game_loop();

void execute_user_command(int command);

bool is_valid_move(node *moved_node, node *destination_node, bool is_to_foundation);

void free_columns();

void shuffle_deck(linked_list *list, int length);

void shuffle_array(node **array, int length);

linked_list **distribute_cards_into_columns_for_game(linked_list *list_copy);

linked_list **distribute_cards_into_columns_for_show(linked_list *list_copy, bool visible);

int get_card_value(char name);

char get_card_name(int value);

#endif //CARD_GAME_GAME_H
