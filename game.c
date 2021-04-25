#include <stdlib.h>
#include <stdio.h>

#include "game.h"

linked_list **distribute_cards_into_columns(linked_list *list) {
    if (!list)
        return NULL;

    static linked_list *columns[NUMBER_OF_COLUMNS];

    for (int i = 0; i < NUMBER_OF_COLUMNS; ++i) {
        columns[i] = malloc(sizeof(linked_list));
        if (!columns[i]) {
            printf("ERROR: malloc failed on column #%d\n", i + 1);
            return columns;
        }
        columns[i]->head = columns[i]->dummy = NULL;
    }

    add_last(remove_last(list), columns[0]);
    columns[0]->head->card->visible = true;

    node *cursor;
    int counter;
    for (int i = NEXT_SHORTEST_COLUMN_LENGTH; i <= LONGEST_COLUMN_LENGTH; ++i) {
        cursor = list->dummy->prev;
        counter = 0;

        while (++counter < i) {
            if (counter <= 5)
                cursor->card->visible = true;

            cursor = cursor->prev;
        }

        move_card_node(cursor, list, columns[i - NEXT_SHORTEST_COLUMN_LENGTH + 1]);
    }

    free(list);

    return columns;
}


int get_card_value(char name) {
    switch (name) {
        case 'A':
            return 1;
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            return name - '0';
        case 'T':
            return 10;
        case 'J':
            return 11;
        case 'Q':
            return 12;
        case 'K':
            return 13;
        default:
            return -1;
    }
}

char get_card_name(int value) {
    switch (value) {
        case 1:
            return 'A';
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
            return '0' + value;
        case 10:
            return 'T';
        case 11:
            return 'J';
        case 12:
            return 'Q';
        case 13:
            return 'K';
        default:
            return -1;
    }
}
