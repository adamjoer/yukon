#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "game.h"
#include "gui.h"

linked_list *deck = NULL;
linked_list *columns[NO_COLUMNS];
linked_list *foundations[NO_FOUNDATIONS];

bool play_phase_active = false;
bool show_columns = false;

bool keep_playing = true;

void game_loop() {

    set_message("Welcome to Yukon");

    while (true) {
        print_board(columns, foundations, show_columns, play_phase_active);
        if (!keep_playing) {
            printf("\n");
            break;
        }

        execute_user_command(get_user_command());
    }

    free_linked_list(deck, true);
    free_columns();
}

static void execute_user_command(enum command command) {

    char *filepath;

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

            if (strlen(argument) > 0) {
                if (validate_file(argument) != OK)
                    break;
            }

            free_linked_list(deck, true);
            free_columns();

            if (strlen(argument) > 0)
                deck = load_from_file(argument, false);
            else
                load_default_deck();

            distribute_cards_into_columns_for_show(deck, false);

            set_message("OK");
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

            for (int i = 0; i < NO_FOUNDATIONS; ++i) {
                foundations[i] = init_linked_list();
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

            filepath = (strlen(argument) > 0) ? argument : "cards.txt";

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
            for (int i = 0; i < NO_FOUNDATIONS && game_won; ++i) {
                if (is_empty(foundations[i]) || last(foundations[i])->value != 13)
                    game_won = false;
            }

            if (game_won)
                set_message("Congratulations, you won!");
            break;

        case SHUFFLE_SPLIT:
            set_message("Command not implemented");
            break;

        case INVALID_INPUT_FORMAT:
            set_message("Unknown command");
            break;

        case ERROR:
        default:
            set_message("Input parser failed");
    }
}

static void load_default_deck() {
    const char ranks[] = {'A', '2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K'};
    const char suits[] = {'C', 'D', 'S', 'H'};

    deck = init_linked_list();

    int ranks_index = 0;
    card *insert_card;
    for (int i = 0; i < 52; ++i) {

        insert_card = malloc(sizeof(card));
        if (!insert_card) {
            perror("load_default_deck");
            exit(1);
        }
        insert_card->rank = ranks[ranks_index];
        insert_card->suit = suits[i / 13];
        insert_card->value = get_card_value(ranks[ranks_index]);
        insert_card->visible = false;

        add_last(insert_card, deck);

        ranks_index = (ranks_index + 1) % 13;
    }
}

static void free_columns() {
    if (!show_columns)
        return;

    for (int i = 0; i < NO_COLUMNS; ++i)
        free_linked_list(columns[i], false);

    show_columns = false;
}

static bool move_card_action() {

    if (!is_valid_column(source_column)) {
        set_message("Invalid source column");
        return false;
    }

    if (destination_column[0] == 'F') {
        if (!is_valid_foundation(destination_column)) {
            set_message("Invalid foundation");
            return false;
        }

    } else {
        if (!is_valid_column(destination_column)) {
            set_message("Invalid destination column");
            return false;
        }
    }

    int source_column_index = source_column[1] - '0' - 1;
    int destination_column_index = destination_column[1] - '0' - 1;
    if (source_column_index == destination_column_index && destination_column[0] != 'F') {
        set_message("Destination column cannot be the same as source column");
        return false;
    }

    node *moved_node;
    node *destination_node;

    if (strlen(moved_card) > 0) {
        if (!is_valid_card(moved_card)) {
            set_message("Invalid card");
            return false;
        }

        moved_node = find_string(moved_card, columns[source_column_index]);
        if (!moved_node || !moved_node->card->visible) {
            set_message("Source column does not contain specified card");
            return false;
        }

    } else {
        if (is_empty(columns[source_column_index])) {
            set_message("Source column empty");
            return false;
        }

        moved_node = columns[source_column_index]->dummy->prev;
    }

    if (destination_column[0] == 'F') {
        if (is_empty(foundations[destination_column_index]))
            destination_node = NULL;
        else
            destination_node = foundations[destination_column_index]->dummy->prev;

    } else {
        if (is_empty(columns[destination_column_index]))
            destination_node = NULL;
        else
            destination_node = columns[destination_column_index]->dummy->prev;
    }

    if (!is_valid_move(moved_node, destination_node, destination_column[0] == 'F')) {
        set_message("Invalid move");
        return false;
    }

    if (destination_column[0] == 'F')
        add_last(remove_last(columns[source_column_index]), foundations[destination_column_index]);
    else
        move_node(moved_node, columns[source_column_index], columns[destination_column_index]);

    if (!is_empty(columns[source_column_index]))
        last(columns[source_column_index])->visible = true;

    set_message("OK");
    return true;
}

static bool is_valid_move(node *moved_node, node *destination_node, bool is_to_foundation) {
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

static void quit_game() {
    play_phase_active = false;
    free_columns();

    for (int i = 0; i < NO_FOUNDATIONS; ++i)
        free_linked_list(foundations[i], false);
}

static void distribute_cards_into_columns_for_game(linked_list *list) {
    if (!list)
        return;

    linked_list *list_copy = copy(list);

    for (int i = 0; i < NO_COLUMNS; ++i) {
        columns[i] = init_linked_list();
    }

    node *cursor;
    int counter;
    for (int i = LONGEST_COLUMN_LENGTH, next_shortest_column_length = LONGEST_COLUMN_LENGTH - (NO_COLUMNS - 2);
         i >= next_shortest_column_length;
         --i) {
        cursor = list_copy->dummy->prev;
        counter = 0;

        while (++counter < i) {
            cursor->card->visible = counter <= 5;
            cursor = cursor->prev;
        }
        cursor->card->visible = false;

        move_node(cursor, list_copy, columns[i - next_shortest_column_length + 1]);
    }

    add_last(remove_last(list_copy), columns[0]);
    columns[0]->head->card->visible = true;

    free_linked_list(list_copy, false);

    show_columns = true;
}

static void distribute_cards_into_columns_for_show(linked_list *list, bool visible) {
    if (!list)
        return;

    linked_list *list_copy = copy(list);

    for (int i = 0; i < NO_COLUMNS; ++i) {
        columns[i] = init_linked_list();
    }

    node *cursor;
    for (int i = NO_COLUMNS - 1, counter, column_length; i >= 0; --i) {
        cursor = list_copy->dummy->prev;
        counter = 0;

        column_length = i < 3 ? 8 : 7;
        while (++counter < column_length) {
            cursor->card->visible = visible;
            cursor = cursor->prev;
        }

        cursor->card->visible = visible;
        move_node(cursor, list_copy, columns[i]);
    }

    free_linked_list(list_copy, false);

    show_columns = true;
}

