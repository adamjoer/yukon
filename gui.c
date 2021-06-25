#include <stdio.h>
#include <stdlib.h>

#include "gui.h"
#include "game.h"

char last_command[IN_BUFFER_SIZE];
char message[MESSAGE_BUFFER_SIZE];

void print_board(linked_list *columns[], linked_list *foundations[]) {
    node *cursors[NUMBER_OF_COLUMNS];

    int longest_column_length = LONGEST_COLUMN_LENGTH;

    if (show_columns) {
        for (int i = 0, len; i < NUMBER_OF_COLUMNS; ++i) {
            len = length(columns[i]);
            if (len > longest_column_length)
                longest_column_length = len;

            cursors[i] = columns[i]->head;
        }
    }

    clear_console();

    for (int i = 0; i < NUMBER_OF_COLUMNS; ++i)
        printf("C%d\t", i + 1);
    printf("\n\n");

    int foundation_counter = 0;
    for (int i = 0; i < longest_column_length; ++i) {

        for (int j = 0; j < NUMBER_OF_COLUMNS; ++j) {
            if (!show_columns || cursors[j] == columns[j]->dummy) {
                printf("\t");
                continue;
            }

            if (cursors[j]->card->visible)
                printf("%c%c\t", cursors[j]->card->rank, cursors[j]->card->suit);
            else
                printf("[]\t");

            cursors[j] = cursors[j]->next;
        }

        if (i % 2 == 0 && foundation_counter < NUMBER_OF_FOUNDATIONS) {
            if (play_phase_active && foundations[foundation_counter]->dummy) {
                printf("\t%c%c\tF%d",
                       last(foundations[foundation_counter])->rank,
                       last(foundations[foundation_counter])->suit,
                       foundation_counter + 1);

            } else {
                printf("\t[]\tF%d", foundation_counter + 1);
            }
            ++foundation_counter;
        }

        printf("\n");
    }

    printf("LAST Command: %s\nMessage: %s\nINPUT> ", last_command, message);
}

void clear_console() {
#ifdef _WIN32
    system("cls");
#else
    printf("\e[1;1H\e[2J");
#endif
}
