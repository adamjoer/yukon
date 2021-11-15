#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "gui.h"

static void print_board_safe();

static void clear_console();

static bool clear_console_before_print;

static char last_command[IN_BUFFER_SIZE];
static char message[MESSAGE_BUFFER_SIZE];

void gui_init(bool clear_console, char *first_last_command, char *first_message) {
    assert(first_last_command != NULL);
    assert(first_message != NULL);

    clear_console_before_print = clear_console;

    set_last_command(first_last_command);
    set_message(first_message);
}

void print_board(LinkedList columns[], LinkedList foundations[]) {
    if (!columns || !foundations) {
        print_board_safe();
        return;
    }

#ifndef NDEBUG
    for (int i = 0; i < NO_COLUMNS; ++i) {
        ASSERT_LINKED_LIST_STRUCT(columns[i]);
    }

    for (int i = 0; i < NO_FOUNDATIONS; ++i) {
        ASSERT_LINKED_LIST_STRUCT(foundations[i]);
    }
#endif

    clear_console();

    Node *cursors[NO_COLUMNS];

    int longest_column_length = LONGEST_COLUMN_LENGTH;

    for (int i = 0, len; i < NO_COLUMNS; ++i) {
        len = length(&columns[i]);
        if (len > longest_column_length)
            longest_column_length = len;

        cursors[i] = columns[i].head;

        printf("C%d\t", i + 1);
    }
    printf("\n\n");

    int foundation_counter = 0;
    for (int i = 0; i < longest_column_length; ++i) {

        for (int j = 0; j < NO_COLUMNS; ++j) {
            if (cursors[j] == columns[j].dummy) {
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
            if (!is_empty(&foundations[foundation_counter])) {
                printf("\t%c%c\tF%d",
                       last(&foundations[foundation_counter])->rank,
                       last(&foundations[foundation_counter])->suit,
                       foundation_counter + 1);

            } else {
                printf("\t[]\tF%d", foundation_counter + 1);
            }
            ++foundation_counter;
        }

        printf("\n");
    }

    printf("LAST Command: %s\n"
           "Message: %s\n"
           "INPUT> ", last_command, message);
}

static void print_board_safe() {
    clear_console();

    for (int i = 0; i < NO_COLUMNS; ++i)
        printf("C%d\t", i + 1);
    printf("\n\n");

    int foundation_counter = 0;
    for (int i = 0; i < LONGEST_COLUMN_LENGTH; ++i) {

        for (int j = 0; j < NO_COLUMNS; ++j)
            printf("\t");

        if (i % 2 == 0 && foundation_counter < NO_FOUNDATIONS)
            printf("\t[]\tF%d", ++foundation_counter);

        printf("\n");
    }

    printf("LAST Command: %s\n"
           "Message: %s\n"
           "INPUT>\n", last_command, message);
}

static void clear_console() {

    if (clear_console_before_print) {
#ifdef _WIN32
        system("cls");
#else
        printf("\e[1;1H\e[2J");
#endif
    } else {
        printf("\n");
    }
}

void generate_columns_game(LinkedList *deck, LinkedList columns[]) {
#ifndef NDEBUG
    ASSERT_LINKED_LIST_REF(deck);

    for (int i = 0; i < NO_COLUMNS; ++i) {
        ASSERT_LINKED_LIST_STRUCT(columns[i]);
    }
#endif

    const int column_lengths[NO_COLUMNS] = {1, 6, 7, 8, 9, 10, 11};

    Node *cursor = deck->head;
    for (int column_index = 0; cursor != deck->dummy; column_index = (column_index + 1) % NO_COLUMNS) {
        if (length(&columns[column_index]) >= column_lengths[column_index])
            continue;

        cursor->card->visible = length(&columns[column_index]) >= column_index;
        add_last(cursor->card, &columns[column_index]);

        cursor = cursor->next;
    }
}

void generate_columns_show(LinkedList *deck, LinkedList columns[], bool visible) {
#ifndef NDEBUG
    ASSERT_LINKED_LIST_REF(deck);

    for (int i = 0; i < NO_COLUMNS; ++i) {
        ASSERT_LINKED_LIST_STRUCT(columns[i]);
    }
#endif

    Node *cursor = deck->head;
    for (int column_index = 0; cursor != deck->dummy; column_index = (column_index + 1) % NO_COLUMNS) {

        cursor->card->visible = visible;
        add_last(cursor->card, &columns[column_index]);

        cursor = cursor->next;
    }
}

void set_message(const char *new_message) {
    assert(new_message);

    // Copy at most MESSAGE_BUFFER_SIZE chars, to avoid buffer overrun
#ifdef _WIN32
    strncpy_s(message, MESSAGE_BUFFER_SIZE, new_message, MESSAGE_BUFFER_SIZE);
#else
    strncpy(message, new_message, MESSAGE_BUFFER_SIZE);
#endif

    // In case the new last command is longer than MESSAGE_BUFFER_SIZE, the null terminator needs to be manually added
    message[MESSAGE_BUFFER_SIZE - 1] = '\0';
}

void set_last_command(const char *new_last_command) {
    assert(new_last_command);

    // Copy at most IN_BUFFER_SIZE chars, to avoid buffer overrun
#ifdef _WIN32
    strncpy_s(last_command, MESSAGE_BUFFER_SIZE, new_last_command, IN_BUFFER_SIZE);
#else
    strncpy(last_command, new_last_command, IN_BUFFER_SIZE);
#endif

    // In case the new last command is longer than ÌN_BUFFER_SIZE, the null terminator needs to be manually added
    last_command[IN_BUFFER_SIZE - 1] = '\0';
}
