#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#include "game.h"
#include "gui.h"
#include "io.h"

linked_list *deck = NULL;
linked_list **columns = NULL;
linked_list *foundations[NUMBER_OF_FOUNDATIONS];
char *filepath = NULL;
bool play_phase_active = false;
bool keep_playing = true;

void game_loop() {
    while (true) {
        print_board(columns, play_phase_active ? foundations : NULL);
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
                filepath = "decks/sorted_deck.txt";

            } else {
                filepath = get_argument();
            }

            if (validate_file(filepath) == 0) {

                free_linked_list(deck, true);

                free_columns();
                columns = NULL;

                deck = load_from_file(filepath);
                set_message("OK");
                columns = distribute_cards_into_columns_for_show(deck, false);

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

            columns = distribute_cards_into_columns_for_game(deck);
            play_phase_active = true;

            for (int i = 0; i < NUMBER_OF_FOUNDATIONS; ++i) {
                foundations[i] = malloc(sizeof(linked_list));
                foundations[i]->head = foundations[i]->dummy = NULL;
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

            columns = distribute_cards_into_columns_for_show(deck, true);

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

            shuffle_deck(deck, length(deck));
            columns = distribute_cards_into_columns_for_show(deck, true);
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
            set_message("OK");
            break;

        case MOVE_CARD:

            if (!play_phase_active) {
                set_message("No active game");
                break;
            }

            if (!deck) {
                set_message("No valid deck loaded");
                break;
            }

            move_card_action();

            bool game_won = true;
            for (int i = 0; i < NUMBER_OF_FOUNDATIONS; ++i) {
                if (!foundations[i]->dummy || last(foundations[i])->value != 13) {
                    game_won = false;
                    break;
                }
            }

            if (game_won) {
                quit_game();
                set_message("Congratulations! You won!");
            }

            break;

        case SHUFFLE_SPLIT:
            // TODO
            set_message("Command not implemented");
            break;

        case WRONG_INPUT_FORMAT:
            set_message("Unknown command");
            break;

        default:
            set_message("Input parser failed");
    }

}

void free_columns() {
    if (!columns)
        return;

    for (int i = 0; i < NUMBER_OF_COLUMNS; ++i)
        free_linked_list(columns[i], false);
}

void move_card_action() {
    char *source_column;
    char *destination_column;
    char *card;
    int source_column_index, destination_column_index;
    node *moved_node;
    node *destination_node;

    source_column = get_source_column();
    destination_column = get_destination_column();
    card = get_moved_card();

    if (!is_valid_column(source_column)) {
        set_message("Invalid source column");
        return;
    }

    if (destination_column[0] == 'F') {
        if (!is_valid_foundation(destination_column)) {
            set_message("Invalid foundation");
            return;
        }

    } else if (!is_valid_column(destination_column)) {
        set_message("Invalid destination column");
        return;
    }

    source_column_index = source_column[1] - '0' - 1;
    destination_column_index = destination_column[1] - '0' - 1;

    if (strlen(card) != 0) {
        if (!is_valid_card(card)) {
            set_message("Invalid card");
            return;
        }
        moved_node = find(card, columns[source_column_index]);
        if (!moved_node || !moved_node->card->visible) {
            set_message("Source column does not contain specified card");
            return;
        }
    } else {
        if (!columns[source_column_index]->dummy) {
            set_message("Source column empty");
            return;
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
        return;
    }
    if (destination_column[0] == 'F')
        move_card(moved_node, columns[source_column_index], foundations[destination_column_index]);
    else
        move_card(moved_node, columns[source_column_index], columns[destination_column_index]);

    set_message("OK");

    if (columns[source_column_index]->dummy) {
        columns[source_column_index]->dummy->prev->card->visible = true;
    }
}

bool is_valid_move(node *moved_node, node *destination_node, bool is_to_foundation) {
    if (!destination_node) {
        if (is_to_foundation)
            return moved_node->card->name == 'A';

        return moved_node->card->name == 'K';
    }

    if (is_to_foundation) {
        if (moved_node->next->card)
            return false;

        return destination_node->card->value == moved_node->card->value - 1 &&
               destination_node->card->suit == moved_node->card->suit;
    } else {
        return destination_node->card->value == moved_node->card->value + 1 &&
               destination_node->card->suit != moved_node->card->suit;;
    }
}

void quit_game() {
    play_phase_active = false;
    free_columns();
    columns = NULL;

    for (int i = 0; i < NUMBER_OF_FOUNDATIONS; ++i) {
        free_linked_list(foundations[i], false);
    }
}

void shuffle_deck(linked_list *list, int length) {
    node *node_pointers[length];

    node *cursor = list->head;
    for (int i = 0; i < length; ++i) {
        node_pointers[i] = cursor;
        cursor = cursor->next;
    }

    shuffle_array(node_pointers, length);

    for (int i = 0; i < length; ++i) {
        if (i == 0) {
            list->head = node_pointers[i];
            node_pointers[i]->prev = list->dummy;
            node_pointers[i]->next = node_pointers[i + 1];

            list->dummy->next = node_pointers[i];

        } else if (i == length - 1) {
            node_pointers[i]->prev = node_pointers[i - 1];
            node_pointers[i]->next = list->dummy;

            list->dummy->prev = node_pointers[i];

        } else {
            node_pointers[i]->prev = node_pointers[i - 1];
            node_pointers[i]->next = node_pointers[i + 1];
        }
    }
}

void shuffle_array(node *array[], int length) {
    srand(time(NULL));

    node *temp;
    size_t random_index;
    for (int i = 0; i < length - 1; ++i) {
        random_index = i + rand() / (RAND_MAX / (length - i) + 1);
        temp = array[random_index];
        array[random_index] = array[i];
        array[i] = temp;
    }
}

linked_list **distribute_cards_into_columns_for_game(linked_list *list) {
    if (!list)
        return NULL;

    linked_list *list_copy = copy_linked_list(list);

    static linked_list *columns[NUMBER_OF_COLUMNS];

    for (int i = 0; i < NUMBER_OF_COLUMNS; ++i) {
        columns[i] = malloc(sizeof(linked_list));
        if (!columns[i]) {
            printf("ERROR: malloc failed on column #%d\n", i + 1);
            return columns;
        }
        columns[i]->head = columns[i]->dummy = NULL;
    }

    node *cursor;
    int counter;
    for (int i = LONGEST_COLUMN_LENGTH; i >= NEXT_SHORTEST_COLUMN_LENGTH; --i) {
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

        move_card(cursor, list_copy, columns[i - NEXT_SHORTEST_COLUMN_LENGTH + 1]);
    }

    add_last(remove_last(list_copy), columns[0]);
    columns[0]->head->card->visible = true;

    free(list_copy);
    return columns;
}

linked_list **distribute_cards_into_columns_for_show(linked_list *list, bool visible) {
    if (!list)
        return NULL;

    linked_list *list_copy = copy_linked_list(list);

    static linked_list *columns[NUMBER_OF_COLUMNS];

    for (int i = 0; i < NUMBER_OF_COLUMNS; ++i) {
        columns[i] = malloc(sizeof(linked_list));
        if (!columns[i]) {
            printf("ERROR: malloc failed on column #%d\n", i + 1);
            return columns;
        }
        columns[i]->head = columns[i]->dummy = NULL;
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
