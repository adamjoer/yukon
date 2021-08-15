#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gui.h"
#include "io.h"

char last_command[IN_BUFFER_SIZE];
char message[MESSAGE_BUFFER_SIZE];

void print_board(linked_list *columns[], linked_list *foundations[], bool show_columns, bool play_phase_active) {
    node *cursors[NO_COLUMNS];

    int longest_column_length = LONGEST_COLUMN_LENGTH;

    if (show_columns) {
        for (int i = 0, len; i < NO_COLUMNS; ++i) {
            len = length(columns[i]);
            if (len > longest_column_length)
                longest_column_length = len;

            cursors[i] = columns[i]->head;
        }
    }

    clear_console();

    for (int i = 0; i < NO_COLUMNS; ++i)
        printf("C%d\t", i + 1);
    printf("\n\n");

    int foundation_counter = 0;
    for (int i = 0; i < longest_column_length; ++i) {

        for (int j = 0; j < NO_COLUMNS; ++j) {
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

        if (i % 2 == 0 && foundation_counter < NO_FOUNDATIONS) {
            if (play_phase_active && !is_empty(foundations[foundation_counter])) {
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

static void clear_console() {
#ifdef _WIN32
    system("cls");
#else
    printf("\e[1;1H\e[2J");
#endif
}

void generate_columns_game(linked_list *deck, linked_list *columns[]) {
    if (!deck)
        return;

    for (int i = 0; i < NO_COLUMNS; ++i)
        columns[i] = init_linked_list();

    int column_lengths[NO_COLUMNS];

    int remaining_cards = 52;
    for (int i = NO_COLUMNS - 1; i > 0; --i) {
        column_lengths[i] = LONGEST_COLUMN_LENGTH - (NO_COLUMNS - (i + 1));
        remaining_cards -= column_lengths[i];
    }

    if (remaining_cards < 0)
        remaining_cards = 0;
    column_lengths[0] = remaining_cards;

    node *cursor = deck->head;
    for (int column_index = 0; cursor != deck->dummy; column_index = (column_index + 1) % NO_COLUMNS) {
        if (length(columns[column_index]) >= column_lengths[column_index])
            continue;

        cursor->card->visible = length(columns[column_index]) >= column_index;
        add_last(cursor->card, columns[column_index]);

        cursor = cursor->next;
    }
}

void generate_columns_show(linked_list *deck, linked_list *columns[], bool visible) {
    if (!deck)
        return;

    for (int i = 0; i < NO_COLUMNS; ++i)
        columns[i] = init_linked_list();

    node *cursor = deck->head;
    for (int column_index = 0; cursor != deck->dummy; column_index = (column_index + 1) % NO_COLUMNS) {

        cursor->card->visible = visible;
        add_last(cursor->card, columns[column_index]);

        cursor = cursor->next;
    }
}

void set_message(char *new_message) {

    // Safety check to avoid buffer overrun
    size_t new_message_length = strlen(new_message);
    if (new_message_length > MESSAGE_BUFFER_SIZE)
        new_message_length = MESSAGE_BUFFER_SIZE;

    strncpy(message, new_message, new_message_length);
    message[new_message_length] = '\0';
}

void set_last_command(char *new_last_command) {

    // Safety check to avoid buffer overrun
    size_t new_last_command_length = strlen(new_last_command);
    if (new_last_command_length > MESSAGE_BUFFER_SIZE)
        new_last_command_length = MESSAGE_BUFFER_SIZE;

    strncpy(last_command, new_last_command, new_last_command_length);
    last_command[new_last_command_length] = '\0';
}