#include <stdio.h>
#include <stdlib.h>

#include "gui.h"
#include "game.h"

void print_board(linked_list **columns) {
    node *cursors[NUMBER_OF_COLUMNS];

    int buffer_length = (LONGEST_COLUMN_LENGTH * (NUMBER_OF_COLUMNS * 3) + 8) + 1;
    char buffer[buffer_length];
    int number_of_chars_written = 0;

    for (int i = 0; i < NUMBER_OF_COLUMNS; ++i) {
        number_of_chars_written += sprintf(buffer + number_of_chars_written, "C%d\t", i + 1);

        if (columns)
            cursors[i] = columns[i]->head;
    }
    number_of_chars_written += sprintf(buffer + number_of_chars_written, "\n\n");

    int foundation_count = 1;
    for (int i = 0; i < LONGEST_COLUMN_LENGTH; ++i) {

        for (int j = 0; j < NUMBER_OF_COLUMNS; ++j) {
            if (!columns || cursors[j] == columns[j]->dummy) {
                number_of_chars_written += sprintf(buffer + number_of_chars_written, "\t");
                continue;
            }

            if (cursors[j]->card->visible)
                number_of_chars_written += sprintf(buffer + number_of_chars_written, "%c%c\t",
                                                   cursors[j]->card->name,
                                                   cursors[j]->card->suit);
            else
                number_of_chars_written += sprintf(buffer + number_of_chars_written, "[]\t");

            cursors[j] = cursors[j]->next;
        }

        if (i <= 6 && i % 2 == 0)
            number_of_chars_written += sprintf(buffer + number_of_chars_written, "\t[]\tF%d", foundation_count++);

        number_of_chars_written += sprintf(buffer + number_of_chars_written, "\n");
    }

    printf("%s", buffer);

    if (number_of_chars_written > buffer_length) {
        printf("WARNING: buffer is not long enough (length: %d, written: %d)\n",
               buffer_length,
               number_of_chars_written);
    }
}

void clear_console() {
#ifdef _WIN32
    system("cls");
#else
    printf("\e[1;1H\e[2J");
#endif
}

