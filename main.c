#include <stdio.h>

#include "linked_list.h"
#include "gui.h"
#include "io.h"
#include "game.h"

int main() {

    linked_list *list = NULL;
    linked_list **columns = NULL;
    char *filepath = NULL;
    bool play_phase_active = false;
    bool keep_playing = true;
    while (true) {
        print_board(columns);
        if (!keep_playing) {
            printf("\n");
            break;
        }

        switch (get_user_command()) {
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

                if (get_argument()[0] == '\0') {
                    filepath = "decks/sorted_deck.txt";

                } else {
                    filepath = get_argument();
                }

                if (list)
                    free_linked_list(list, true);
                if (columns) {
                    for (int i = 0; i < NUMBER_OF_COLUMNS; ++i) {
                        free_linked_list(columns[i], false);
                    }
                    columns = NULL;
                }

                if (validate_file(filepath) == 0) {
                    list = load_from_file(filepath);
                    set_message("OK");
                    columns = distribute_cards_into_columns_for_show(list, false);

                }
                break;

            case PLAY:
                if (!list) {
                    set_message("No valid deck loaded");
                    break;
                }

                if (play_phase_active) {
                    set_message("Game already active");
                    break;
                }

                for (int i = 0; i < NUMBER_OF_COLUMNS; ++i) {
                    free_linked_list(columns[i], false);
                }

                columns = distribute_cards_into_columns_for_game(list);
                play_phase_active = true;
                set_message("OK");
                break;

            case QUIT_GAME:
                if (!play_phase_active) {
                    set_message("No active game");
                    break;
                }

                play_phase_active = false;
                for (int i = 0; i < NUMBER_OF_COLUMNS; ++i) {
                    free_linked_list(columns[i], false);
                }
                columns = NULL;
                set_message("OK");
                break;

            case SHOW_CARDS:
                if (play_phase_active) {
                    set_message("Command not available in play phase");
                    break;
                }

                if (!list) {
                    set_message("No valid deck loaded");
                    break;
                }

                if (columns) {
                    for (int i = 0; i < NUMBER_OF_COLUMNS; ++i) {
                        free_linked_list(columns[i], false);
                    }
                }

                columns = distribute_cards_into_columns_for_show(list, true);

                set_message("OK");
                break;

            case SHUFFLE_RANDOM:
                if (play_phase_active) {
                    set_message("Command not available in play phase");
                    break;
                }

                if (!list) {
                    set_message("No valid deck loaded");
                    break;
                }

                if (columns) {
                    for (int i = 0; i < NUMBER_OF_COLUMNS; ++i) {
                        free_linked_list(columns[i], false);
                    }
                }

                shuffle_deck(list, length(list));
                columns = distribute_cards_into_columns_for_show(list, true);
                set_message("OK");
                break;

            case SAVE_DECK:
                if (play_phase_active) {
                    set_message("Command not available in play phase");
                    break;
                }

                if (!list) {
                    set_message("No valid deck loaded");
                    break;
                }

                if (get_argument()[0] == '\0') {
                    filepath = "cards.txt";

                } else {
                    filepath = get_argument();
                }

                save_deck_to_file(list, filepath);
                set_message("OK");
                break;

            case MOVE_CARD:
                // TODO
                set_message("Command not implemented");
                break;

            case WRONG_INPUT_FORMAT:
                set_message("Unknown command");
                break;
        }
    }

    free_linked_list(list, false);

    if (columns) {
        for (int i = 0; i < NUMBER_OF_COLUMNS; ++i) {
            free_linked_list(columns[i], true);
        }
    }

    return 0;
}
