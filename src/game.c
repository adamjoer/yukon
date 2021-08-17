#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "game.h"
#include "gui.h"

linked_list *deck = NULL;
linked_list *columns[NO_COLUMNS];
linked_list *foundations[NO_FOUNDATIONS];

bool play_phase_active = false;
bool show_columns = false;

bool keep_playing = true;

void game_loop() {
    srand(time(NULL));

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

            generate_columns_show(deck, columns, false);
            show_columns = true;

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
            generate_columns_game(deck, columns);
            show_columns = true;

            play_phase_active = true;

            for (int i = 0; i < NO_FOUNDATIONS; ++i)
                foundations[i] = init_linked_list();

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
            generate_columns_show(deck, columns, true);
            show_columns = true;

            set_message("OK");
            break;

        case SHUFFLE_SPLIT:
            if (play_phase_active) {
                set_message("Command not available in play phase");
                break;
            }

            if (!deck) {
                set_message("No valid deck loaded");
                break;
            }

            shuffle_split();

            free_columns();
            generate_columns_show(deck, columns, true);
            show_columns = true;

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

            shuffle_linked_list(deck);

            free_columns();
            generate_columns_show(deck, columns, true);
            show_columns = true;

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

            if (!move_card())
                break;

            bool game_won = true;
            for (int i = 0; i < NO_FOUNDATIONS && game_won; ++i) {
                if (is_empty(foundations[i]) || last(foundations[i])->value != 13)
                    game_won = false;
            }

            if (game_won)
                set_message("Congratulations, you won!");
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
    const char suits[] = {'C', 'D', 'H', 'S'};

    deck = init_linked_list();

    card *insert_card;
    for (int i = 0; i < 52; ++i) {

        insert_card = malloc(sizeof(card));
        if (!insert_card)
            return;

        insert_card->rank = ranks[i % 13];
        insert_card->suit = suits[i / 13];
        insert_card->value = get_card_value(insert_card->rank);
        insert_card->visible = false;

        add_last(insert_card, deck);
    }
}

static void shuffle_split() {
    if (!deck)
        return;

    long split;

    if (strlen(argument) > 0) {
        char *validation_ptr;
        split = strtol(argument, &validation_ptr, 0);

        if (*validation_ptr != '\0') {
            char message_buffer[MESSAGE_BUFFER_SIZE];
            sprintf(message_buffer, "'%s' is not a valid number", argument);
            set_message(message_buffer);
            return;
        }

        if (split < 0 || split > length(deck)) {
            set_message("Split needs to be positive and less than the deck length");
            return;
        }

    } else {
        split = rand() % (length(deck) + 1);
    }

    if (split == 0 || split == length(deck)) {
        set_message("No shuffling necessary");
        return;
    }

    linked_list *first_pile = init_linked_list();
    linked_list *second_pile = init_linked_list();

    node *cursor = deck->head;
    for (int i = 0; i < split; ++i)
        cursor = cursor->next;

    move_node(cursor, deck, second_pile);
    move_node(deck->head, deck, first_pile);

    while (!is_empty(first_pile) && !is_empty(second_pile)) {
        add_last(remove_first(first_pile), deck);
        add_last(remove_first(second_pile), deck);
    }

    while (!is_empty(first_pile))
        add_last(remove_first(first_pile), deck);

    while (!is_empty(second_pile))
        add_last(remove_first(second_pile), deck);

    free_linked_list(first_pile, false);
    free_linked_list(second_pile, false);

    set_message("OK");
}

static void free_columns() {
    if (!show_columns)
        return;

    for (int i = 0; i < NO_COLUMNS; ++i)
        free_linked_list(columns[i], false);

    show_columns = false;
}

static bool move_card() {

    bool from_foundation = source_column[0] == 'F';
    if (from_foundation) {
        if (!is_valid_foundation(source_column)) {
            set_message("Invalid source foundation");
            return false;
        }

    } else {
        if (!is_valid_column(source_column)) {
            set_message("Invalid source column");
            return false;
        }
    }

    bool to_foundation = destination_column[0] == 'F';
    if (to_foundation) {
        if (!is_valid_foundation(destination_column)) {
            set_message("Invalid destination foundation");
            return false;
        }

    } else {
        if (!is_valid_column(destination_column)) {
            set_message("Invalid destination column");
            return false;
        }
    }

    int source_index = source_column[1] - '0' - 1;
    int destination_index = destination_column[1] - '0' - 1;
    if (source_index == destination_index && from_foundation == to_foundation) {
        set_message("Destination cannot be the same as source");
        return false;
    }

    linked_list *source_list = from_foundation ? foundations[source_index] : columns[source_index];
    linked_list *destination_list = to_foundation ? foundations[destination_index] : columns[destination_index];

    node *moving_node;
    node *destination_node;

    if (strlen(moved_card) > 0) {
        if (!is_valid_card(moved_card)) {
            set_message("Invalid card");
            return false;
        }

        moving_node = find_string(moved_card, source_list);
        if (!moving_node || !moving_node->card->visible) {
            set_message("Source column/foundation does not contain specified card");
            return false;
        }

    } else {
        if (is_empty(source_list)) {
            set_message("Source column/foundation empty");
            return false;
        }

        moving_node = source_list->dummy->prev;
    }

    destination_node = !is_empty(destination_list) ? destination_list->dummy->prev : NULL;

    if (!is_valid_move(moving_node, destination_node, from_foundation, to_foundation)) {
        set_message("Invalid move");
        return false;
    }

    if (from_foundation || to_foundation)
        add_last(remove_last(source_list), destination_list);
    else
        move_node(moving_node, source_list, destination_list);

    if (!is_empty(source_list))
        last(source_list)->visible = true;

    set_message("OK");
    return true;
}

static bool is_valid_move(node *moving_node, node *destination_node, bool from_foundation, bool to_foundation) {
    if ((from_foundation || to_foundation) && moving_node->next->card)
        return false;

    if (!destination_node) {
        if (to_foundation)
            return moving_node->card->value == 1;

        return moving_node->card->value == 13;
    }

    if (to_foundation) {
        return destination_node->card->value == moving_node->card->value - 1 &&
               destination_node->card->suit == moving_node->card->suit;
    }

    return destination_node->card->value == moving_node->card->value + 1 &&
           destination_node->card->suit != moving_node->card->suit;
}

static void quit_game() {
    if (!play_phase_active)
        return;

    play_phase_active = false;

    free_columns();
    generate_columns_show(deck, columns, false);
    show_columns = true;

    for (int i = 0; i < NO_FOUNDATIONS; ++i)
        free_linked_list(foundations[i], false);
}
