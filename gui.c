#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gui.h"
#include "game.h"

void print_board(linked_list *columns[]) {
    node *cursors[NUMBER_OF_COLUMNS];

    int longest_column_length = 0;
    for (int i = 0, len; i < NUMBER_OF_COLUMNS; ++i) {
        len = length(columns[i]);
        if (len > longest_column_length)
            longest_column_length = len;
    }


    int buffer_length = (longest_column_length * (NUMBER_OF_COLUMNS * 3) + 8) +
                        32 +
                        (last_command ? (int) strlen(last_command) : 0) +
                        (message ? (int) strlen(message) : 0);
    char buffer[buffer_length];
    int number_of_chars_written = 0;

    for (int i = 0; i < NUMBER_OF_COLUMNS; ++i) {
        number_of_chars_written += sprintf(buffer + number_of_chars_written, "C%d\t", i + 1);

        if (columns)
            cursors[i] = columns[i]->head;
    }
    number_of_chars_written += sprintf(buffer + number_of_chars_written, "\n\n");

    int foundation_count = 1;
    for (int i = 0; i < longest_column_length; ++i) {

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

    if (last_command && message) {
        number_of_chars_written += sprintf(buffer + number_of_chars_written, "\nLAST Command: %s\n", last_command);
        number_of_chars_written += sprintf(buffer + number_of_chars_written, "Message: %s\n", message);
        last_command = message = NULL;

    } else {
        number_of_chars_written += sprintf(buffer + number_of_chars_written, "\nLAST Command:\n");
        number_of_chars_written += sprintf(buffer + number_of_chars_written, "Message:\n");
    }
    number_of_chars_written += sprintf(buffer + number_of_chars_written, "INPUT >");

    printf("%s", buffer);

    printf("buffer_length=%d, number_of_chars_written=%d\n", buffer_length, number_of_chars_written);

    if (number_of_chars_written > buffer_length) {
        printf("\nWARNING: buffer is not long enough (length: %d, written: %d)\n",
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

