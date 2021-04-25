#ifndef CARD_GAME_GAME_H
#define CARD_GAME_GAME_H

#define NUMBER_OF_COLUMNS 7
#define NEXT_SHORTEST_COLUMN_LENGTH 6
#define LONGEST_COLUMN_LENGTH 11

#include "linked_list.h"

void shuffle_deck(linked_list *list, int length);

void shuffle_array(node **array, int length);

linked_list **distribute_cards_into_columns(linked_list *list);

int get_card_value(char name);

char get_card_name(int value);

#endif //CARD_GAME_GAME_H
