#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <assert.h>

#include "game.h"
#include "gui.h"

static void game_init();

static void game_destroy();

static void game_loop();

static void abort_handler(int signal);

static void interrupt_handler(int signal);

static void execute_user_command(enum Command command);

static void load_default_deck();

static void shuffle_split();

static bool move_card();

static bool is_valid_move(Node *moving_node, Node *destination_node, bool from_foundation, bool to_foundation);

static void empty_columns();

static void quit_game();

LinkedList deck;
LinkedList columns[NO_COLUMNS];
LinkedList foundations[NO_FOUNDATIONS];

bool game_initialised = false;
bool play_phase_active = false;
bool show_columns = false;
bool keep_playing = true;

void start_game() {
    game_init();

    game_loop();

    game_destroy();
}

static void game_init() {
    if (game_initialised)
        return;

    signal(SIGABRT, abort_handler);
    signal(SIGINT, interrupt_handler);

    srand(time(NULL));

    linked_list_init(&deck);

    for (int i = 0; i < NO_COLUMNS; ++i)
        linked_list_init(&columns[i]);

    for (int i = 0; i < NO_FOUNDATIONS; ++i)
        linked_list_init(&foundations[i]);

    set_message("Welcome to Yukon");

    game_initialised = true;
}

static void game_destroy() {
    if (!game_initialised)
        return;

    linked_list_destroy(&deck, true);

    for (int i = 0; i < NO_COLUMNS; ++i)
        linked_list_destroy(&columns[i], false);

    for (int i = 0; i < NO_FOUNDATIONS; ++i)
        linked_list_destroy(&foundations[i], false);

    game_initialised = false;
}

static void game_loop() {
    while (true) {
        print_board(columns, foundations);

        if (!keep_playing) {
            printf("\n");
            break;
        }

        execute_user_command(get_user_command());
    }
}

static void abort_handler(int signal) {
    set_message("Oh shit, something fucked up!");
    print_board(NULL, NULL);

    game_destroy();
    exit(1);
}

static void interrupt_handler(int signal) {
    set_last_command("*Program interrupted*");
    set_message("Goodbye!");
    print_board(NULL, NULL);

    game_destroy();
    exit(0);
}

static void execute_user_command(enum Command command) {

    switch (command) {
        case QuitProgram:
            if (play_phase_active) {
                set_message("Command not available in play phase");
                break;
            }

            set_message("Goodbye!");
            keep_playing = false;
            break;

        case LoadFile:
            if (play_phase_active) {
                set_message("Command not available in play phase");
                break;
            }

            if (strlen(argument) > 0) {
                if (validate_file(argument) != OK)
                    break;
            }

            empty_linked_list(&deck, true);
            empty_columns();

            if (strlen(argument) > 0)
                load_from_file(&deck, argument, false);
            else
                load_default_deck();

            generate_columns_show(&deck, columns, false);
            show_columns = true;

            set_message("OK");
            break;

        case Play:
            if (length(&deck) == 0) {
                set_message("No valid deck loaded");
                break;
            }

            if (play_phase_active) {
                set_message("Game already active");
                break;
            }

            empty_columns();
            generate_columns_game(&deck, columns);
            show_columns = true;

            play_phase_active = true;

            set_message("OK");
            break;

        case QuitGame:
            if (!play_phase_active) {
                set_message("No active game");
                break;
            }

            quit_game();

            set_message("OK");
            break;

        case ShowCards:
            if (play_phase_active) {
                set_message("Command not available in play phase");
                break;
            }

            if (length(&deck) == 0) {
                set_message("No valid deck loaded");
                break;
            }

            empty_columns();
            generate_columns_show(&deck, columns, true);
            show_columns = true;

            set_message("OK");
            break;

        case ShuffleSplit:
            if (play_phase_active) {
                set_message("Command not available in play phase");
                break;
            }

            if (length(&deck) == 0) {
                set_message("No valid deck loaded");
                break;
            }

            shuffle_split();

            empty_columns();
            generate_columns_show(&deck, columns, true);
            show_columns = true;

            break;

        case ShuffleRandom:
            if (play_phase_active) {
                set_message("Command not available in play phase");
                break;
            }

            if (length(&deck) == 0) {
                set_message("No valid deck loaded");
                break;
            }

            shuffle_linked_list(&deck);

            empty_columns();
            generate_columns_show(&deck, columns, true);
            show_columns = true;

            set_message("OK");
            break;

        case SaveDeck:
            if (play_phase_active) {
                set_message("Command not available in play phase");
                break;
            }

            if (length(&deck) == 0) {
                set_message("No valid deck loaded");
                break;
            }

            save_deck_to_file(&deck, strlen(argument) > 0 ? argument : "cards.txt");
            break;

        case MoveCard:
            if (!play_phase_active) {
                set_message("No active game");
                break;
            }

            if (!move_card())
                break;

            bool game_won = true;
            for (int i = 0; i < NO_FOUNDATIONS && game_won; ++i) {
                if (is_empty(&foundations[i]) || last(&foundations[i])->value != 13)
                    game_won = false;
            }

            if (game_won)
                set_message("Congratulations, you won!");
            break;

        case InvalidInputFormat:
            set_message("Unknown Command");
            break;

        case Error:
        default:
            set_message("Input parser failed");
            break;
    }
}

static void load_default_deck() {
    const char ranks[] = {'A', '2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K'};
    const char suits[] = {'C', 'D', 'H', 'S'};

    Card *insert_card;
    for (int i = 0; i < 52; ++i) {

        insert_card = malloc(sizeof(Card));
        assert(insert_card != NULL);

        insert_card->rank = ranks[i % 13];
        insert_card->suit = suits[i / 13];
        insert_card->value = get_card_value(insert_card->rank);
        insert_card->visible = false;

        add_last(insert_card, &deck);
    }
}

static void shuffle_split() {
    int deck_length = length(&deck);

    if (deck_length == 0)
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

        if (split < 0 || split > deck_length) {
            set_message("Split needs to be positive and less than the deck length");
            return;
        }

    } else {
        split = rand() % (deck_length + 1);
    }

    if (split == 0 || split == deck_length)
        goto out;

    LinkedList first_pile;
    LinkedList second_pile;
    linked_list_init(&first_pile);
    linked_list_init(&second_pile);

    Node *cursor = deck.head;
    for (int i = 0; i < split; ++i)
        cursor = cursor->next;

    move_node(cursor, &deck, &second_pile);
    move_node(deck.head, &deck, &first_pile);

    while (!is_empty(&first_pile) && !is_empty(&second_pile)) {
        add_last(remove_first(&first_pile), &deck);
        add_last(remove_first(&second_pile), &deck);
    }

    while (!is_empty(&first_pile))
        add_last(remove_first(&first_pile), &deck);

    while (!is_empty(&second_pile))
        add_last(remove_first(&second_pile), &deck);

    linked_list_destroy(&first_pile, false);
    linked_list_destroy(&second_pile, false);

out:
    set_message("OK");
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

    LinkedList *source_list = from_foundation ? &foundations[source_index] : &columns[source_index];
    LinkedList *destination_list = to_foundation ? &foundations[destination_index] : &columns[destination_index];

    Node *moving_node;

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

    Node *destination_node = !is_empty(destination_list) ? destination_list->dummy->prev : NULL;

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

static bool is_valid_move(Node *moving_node, Node *destination_node, bool from_foundation, bool to_foundation) {
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

static void empty_columns() {
    if (!show_columns)
        return;

    for (int i = 0; i < NO_COLUMNS; ++i)
        empty_linked_list(&columns[i], false);

    show_columns = false;
}

static void quit_game() {
    if (!play_phase_active)
        return;

    empty_columns();
    generate_columns_show(&deck, columns, false);
    show_columns = true;

    for (int i = 0; i < NO_FOUNDATIONS; ++i)
        empty_linked_list(&foundations[i], false);

    play_phase_active = false;
}
