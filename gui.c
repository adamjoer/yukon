#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gui.h"
#include "io.h"

char last_command[IN_BUFFER_SIZE];
char message[MESSAGE_BUFFER_SIZE];

void print_board(linked_list *columns[], linked_list *foundations[]) {
    node *cursors[NUMBER_OF_COLUMNS];

    int longest_column_length = LONGEST_COLUMN_LENGTH;

    if (columns) {
        for (int i = 0, len; i < NUMBER_OF_COLUMNS; ++i) {
            len = length(columns[i]);
            if (len > longest_column_length)
                longest_column_length = len;
        }
    }

    clear_console();

    for (int i = 0; i < NUMBER_OF_COLUMNS; ++i) {
        printf("C%d\t", i + 1);

        if (columns)
            cursors[i] = columns[i]->head;
    }
    printf("\n\n");

    int foundation_counter = 0;
    for (int i = 0; i < longest_column_length; ++i) {

        for (int j = 0; j < NUMBER_OF_COLUMNS; ++j) {
            if (!columns || cursors[j] == columns[j]->dummy) {
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
            if (foundations && foundations[foundation_counter]->dummy) {
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

    printf("LAST Command: %s\nMessage: %s\nINPUT > ", last_command, message);
}

void clear_console() {
#ifdef _WIN32
    system("cls");
#else
    printf("\e[1;1H\e[2J");
#endif
}

void set_message(char *new_message) {
    strcpy(message, new_message);
}

void set_last_command(char *new_last_command) {
    strcpy(last_command, new_last_command);
}
