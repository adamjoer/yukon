#ifndef CARD_GAME_GAME_H
#define CARD_GAME_GAME_H

#define NUMBER_OF_COLUMNS 7
#define NEXT_SHORTEST_COLUMN_LENGTH 6
#define LONGEST_COLUMN_LENGTH 11

#include "linked_list.h"

linked_list **distribute_cards_into_columns(linked_list *list);

int get_card_value(char name);

char get_card_name(int value);

#endif //CARD_GAME_GAME_H
