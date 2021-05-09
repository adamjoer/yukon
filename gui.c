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

    char buffer[OUT_BUFFER_SIZE];
    int number_of_chars_written = 0;

    for (int i = 0; i < NUMBER_OF_COLUMNS; ++i) {
        number_of_chars_written += sprintf(buffer + number_of_chars_written, "C%d\t", i + 1);

        if (columns)
            cursors[i] = columns[i]->head;
    }
    number_of_chars_written += sprintf(buffer + number_of_chars_written, "\n\n");

    int foundation_counter = 0;
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

        if (i % 2 == 0 && foundation_counter < NUMBER_OF_FOUNDATIONS) {
            if (foundations && foundations[foundation_counter]->dummy) {
                number_of_chars_written += sprintf(buffer + number_of_chars_written, "\t%c%c\tF%d",
                                                   last(foundations[foundation_counter])->name,
                                                   last(foundations[foundation_counter])->suit,
                                                   foundation_counter + 1);

            } else {
                number_of_chars_written += sprintf(buffer + number_of_chars_written, "\t[]\tF%d", foundation_counter + 1);
            }
            ++foundation_counter;
        }

        number_of_chars_written += sprintf(buffer + number_of_chars_written, "\n");
    }

    number_of_chars_written += sprintf(buffer + number_of_chars_written, "\nLAST Command: %s\n", last_command);
    number_of_chars_written += sprintf(buffer + number_of_chars_written, "Message: %s\n", message);
    number_of_chars_written += sprintf(buffer + number_of_chars_written, "INPUT > ");

    clear_console();

    printf("%s", buffer);

    if (number_of_chars_written > OUT_BUFFER_SIZE) {
        printf("\nWARNING: buffer is not long enough (length: %d, written: %d)\n",
               OUT_BUFFER_SIZE,
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

void set_message(char *new_message) {
    strcpy(message, new_message);
}

void set_last_command(char *new_last_command) {
    strcpy(last_command, new_last_command);
}
