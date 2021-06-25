#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "game.h"
#include "gui.h"
#include "io.h"

linked_list *deck = NULL;
linked_list *columns[NUMBER_OF_COLUMNS];
linked_list *foundations[NUMBER_OF_FOUNDATIONS];
char *filepath = NULL;
bool play_phase_active = false, keep_playing = true, show_columns = false;

void game_loop() {

    set_message("Welcome to Yukon");

    while (true) {
        print_board(show_columns ? columns : NULL, play_phase_active ? foundations : NULL);
        if (!keep_playing) {
            printf("\n");
            break;
        }

        execute_user_command(get_user_command());
    }

    free_linked_list(deck, true);
    free_columns();
}

void execute_user_command(int command) {

    switch (command) {
        case QUIT_PROGRAM:
            if (play_phase_active) {
                set_message("Command not available in play phase");
                break;
            }

            set_message("Goodbye!");
            keep_playing = false;
            break;

        case LOAD_FILE:
            if (play_phase_active) {
                set_message("Command not available in play phase");
                break;
            }

            if (strlen(get_argument()) == 0) {
                filepath = "decks/00.txt";

            } else {
                filepath = get_argument();
            }

            if (validate_file(filepath) == 0) {

                free_linked_list(deck, true);

                free_columns();

                deck = load_from_file(filepath, false);
                distribute_cards_into_columns_for_show(deck, false);

                set_message("OK");
            }
            break;

        case PLAY:
            if (!deck) {
                set_message("No valid deck loaded");
                break;
            }

            if (play_phase_active) {
                set_message("Game already active");
                break;
            }

            free_columns();

            distribute_cards_into_columns_for_game(deck);
            play_phase_active = true;

            for (int i = 0; i < NUMBER_OF_FOUNDATIONS; ++i) {
                foundations[i] = malloc(sizeof(linked_list));
                if (!foundations[i]) {
                    perror("execute_user_command");
                    exit(1);
                }

                foundations[i]->head = foundations[i]->dummy = NULL;
                foundations[i]->length = -1;
            }

            set_message("OK");
            break;

        case QUIT_GAME:
            if (!play_phase_active) {
                set_message("No active game");
                break;
            }

            quit_game();

            set_message("OK");
            break;

        case SHOW_CARDS:
            if (play_phase_active) {
                set_message("Command not available in play phase");
                break;
            }

            if (!deck) {
                set_message("No valid deck loaded");
                break;
            }

            free_columns();

            distribute_cards_into_columns_for_show(deck, true);

            set_message("OK");
            break;

        case SHUFFLE_RANDOM:
            if (play_phase_active) {
                set_message("Command not available in play phase");
                break;
            }

            if (!deck) {
                set_message("No valid deck loaded");
                break;
            }

            free_columns();

            shuffle_linked_list(deck);
            distribute_cards_into_columns_for_show(deck, true);
            set_message("OK");
            break;

        case SAVE_DECK:
            if (play_phase_active) {
                set_message("Command not available in play phase");
                break;
            }

            if (!deck) {
                set_message("No valid deck loaded");
                break;
            }

            if (strlen(get_argument()) == 0) {
                filepath = "cards.txt";

            } else {
                filepath = get_argument();
            }

            save_deck_to_file(deck, filepath);
            break;

        case MOVE_CARD:
            if (!play_phase_active) {
                set_message("No active game");
                break;
            }

            if (!move_card_action())
                break;

            bool game_won = true;
            for (int i = 0; i < NUMBER_OF_FOUNDATIONS && game_won; ++i) {
                if (!foundations[i]->dummy || last(foundations[i])->value != 13)
                    game_won = false;
            }

            if (game_won) {
                quit_game();
                set_message("Congratulations! You won!");
            }

            break;

        case SHUFFLE_SPLIT:
            set_message("Command not implemented");
            break;

        case INVALID_INPUT_FORMAT:
            set_message("Unknown command");
            break;

        default:
            set_message("Input parser failed");
    }
}

void free_columns() {
    if (!show_columns)
        return;

    for (int i = 0; i < NUMBER_OF_COLUMNS; ++i)
        free_linked_list(columns[i], false);

    show_columns = false;
}

bool move_card_action() {
    char *source_column = get_source_column();
    char *destination_column = get_destination_column();
    char *card = get_moved_card();

    if (!is_valid_column(source_column)) {
        set_message("Invalid source column");
        return false;
    }

    if (destination_column[0] == 'F') {
        if (!is_valid_foundation(destination_column)) {
            set_message("Invalid foundation");
            return false;
        }

    } else if (!is_valid_column(destination_column)) {
        set_message("Invalid destination column");
        return false;
    }

    int source_column_index = source_column[1] - '0' - 1;
    int destination_column_index = destination_column[1] - '0' - 1;
    if (source_column_index == destination_column_index && destination_column[0] != 'F') {
        set_message("Destination column cannot be the same as source column");
        return false;
    }

    node *moved_node;
    node *destination_node;

    if (strlen(card) != 0) {
        if (!is_valid_card(card)) {
            set_message("Invalid card");
            return false;
        }
        moved_node = find(card, columns[source_column_index]);
        if (!moved_node || !moved_node->card->visible) {
            set_message("Source column does not contain specified card");
            return false;
        }
    } else {
        if (!columns[source_column_index]->dummy) {
            set_message("Source column empty");
            return false;
        }
        moved_node = columns[source_column_index]->dummy->prev;
    }

    if (destination_column[0] == 'F') {

        if (!foundations[destination_column_index]->dummy)
            destination_node = NULL;
        else
            destination_node = foundations[destination_column_index]->dummy->prev;

    } else {

        if (!columns[destination_column_index]->dummy)
            destination_node = NULL;
        else
            destination_node = columns[destination_column_index]->dummy->prev;
    }

    if (!is_valid_move(moved_node, destination_node, destination_column[0] == 'F')) {
        set_message("Invalid move");
        return false;
    }
    if (destination_column[0] == 'F')
        move_card(moved_node, columns[source_column_index], foundations[destination_column_index]);
    else
        move_card(moved_node, columns[source_column_index], columns[destination_column_index]);

    set_message("OK");

    if (columns[source_column_index]->dummy) {
        last(columns[source_column_index])->visible = true;
    }

    return true;
}

bool is_valid_move(node *moved_node, node *destination_node, bool is_to_foundation) {
    if (!destination_node) {
        if (is_to_foundation)
            return !moved_node->next->card && moved_node->card->value == 1;

        return moved_node->card->value == 13;
    }

    if (is_to_foundation) {
        return !moved_node->next->card &&
               destination_node->card->value == moved_node->card->value - 1 &&
               destination_node->card->suit == moved_node->card->suit;
    }

    return destination_node->card->value == moved_node->card->value + 1 &&
           destination_node->card->suit != moved_node->card->suit;
}

void quit_game() {
    play_phase_active = false;
    free_columns();

    for (int i = 0; i < NUMBER_OF_FOUNDATIONS; ++i) {
        free_linked_list(foundations[i], false);
    }
}

void distribute_cards_into_columns_for_game(linked_list *list) {
    if (!list)
        return;

    linked_list *list_copy = copy_linked_list(list);

    for (int i = 0; i < NUMBER_OF_COLUMNS; ++i) {
        columns[i] = malloc(sizeof(linked_list));
        if (!columns[i]) {
            perror("distribute_cards_into_columns_for_game");
            exit(1);
        }

        columns[i]->head = columns[i]->dummy = NULL;
        columns[i]->length = -1;
    }

    node *cursor;
    int counter;
    for (int i = LONGEST_COLUMN_LENGTH, n = LONGEST_COLUMN_LENGTH - (NUMBER_OF_COLUMNS - 2); i >= n; --i) {
        cursor = list_copy->dummy->prev;
        counter = 0;

        while (++counter < i) {
            if (counter <= 5)
                cursor->card->visible = true;
            else
                cursor->card->visible = false;

            cursor = cursor->prev;
        }
        cursor->card->visible = false;

        move_card(cursor, list_copy, columns[i - n + 1]);
    }

    add_last(remove_last(list_copy), columns[0]);
    columns[0]->head->card->visible = true;

    free(list_copy);

    show_columns = true;
}

void distribute_cards_into_columns_for_show(linked_list *list, bool visible) {
    if (!list)
        return;

    linked_list *list_copy = copy_linked_list(list);

    for (int i = 0; i < NUMBER_OF_COLUMNS; ++i) {
        columns[i] = malloc(sizeof(linked_list));
        if (!columns[i]) {
            perror("distribute_cards_into_columns_for_show");
            exit(1);
        }

        columns[i]->head = columns[i]->dummy = NULL;
        columns[i]->length = -1;
    }

    node *cursor;
    int counter, column_length;
    for (int i = NUMBER_OF_COLUMNS - 1; i >= 0; --i) {
        cursor = list_copy->dummy->prev;
        counter = 0;

        if (i < 3)
            column_length = 8;
        else
            column_length = 7;

        while (++counter < column_length) {
            cursor->card->visible = visible;
            cursor = cursor->prev;
        }

        cursor->card->visible = visible;
        move_card(cursor, list_copy, columns[i]);
    }

    free(list_copy);

    show_columns = true;
}

int get_card_value(char rank) {
    switch (rank) {
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
            return rank - '0';
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
