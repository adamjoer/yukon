#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "io.h"
#include "game.h"
#include "gui.h"

char moved_card[2];

char source_column[2];

char destination_column[2];

char argument[64];

linked_list *load_from_file(char *filepath) {
    FILE *file = fopen(filepath, "r");

    linked_list *list = malloc(sizeof(linked_list));
    list->head = list->dummy = NULL;

    char name, suit;

    card *new_card;
    while (fscanf(file, "%c%c\n", &name, &suit) != EOF) {
        new_card = malloc(sizeof(card));
        new_card->name = name;
        new_card->suit = suit;
        new_card->value = get_card_value(name);
        new_card->visible = false;

        add_last(new_card, list);
    }

    fclose(file);

    return list;
}

int validate_file(char *filepath) {

    static char output_buffer[512];

    FILE *file = fopen(filepath, "r");
    if (!file) {
        sprintf(output_buffer, "File '%s' not found", filepath);
        set_message(output_buffer);
        return FILE_NOT_FOUND;
    }

    char buffer[128];
    buffer[2] = '\0';

    int card_value;
    int line_number = 1;

    int suit_count[4];
    for (int i = 0; i < 4; ++i)
        suit_count[i] = 0;

    int card_count[13];
    for (int i = 0; i < 13; ++i)
        card_count[i] = 0;

    while (fscanf(file, "%[^\r\n] ", buffer) != EOF) {
        if (strlen(buffer) != 2) {
            sprintf(output_buffer,
                    "Unknown card format '%s' on line %d: Valid format is [name-char][suit-char] e.g. TH for ten of hearts",
                    buffer, line_number);
            set_message(output_buffer);
            fclose(file);
            return UNKNOWN_FORMAT;
        }

        card_value = get_card_value(buffer[0]);
        if (card_value == -1) {
            sprintf(output_buffer, "Unknown card name '%c' on line %d: Valid names are A, 2-9, T, J, Q, K", buffer[0],
                    line_number);
            set_message(output_buffer);
            fclose(file);
            return UNKNOWN_NAME;
        }

        switch (buffer[1]) {
            case 'C':
                ++suit_count[0];
                break;
            case 'D':
                ++suit_count[1];
                break;
            case 'H':
                ++suit_count[2];
                break;
            case 'S':
                ++suit_count[3];
                break;
            default:
                sprintf(output_buffer, "Unknown card suit '%c' on line %d: Valid suits are C, D, H, S", buffer[1],
                        line_number);
                set_message(output_buffer);
                fclose(file);
                return UNKNOWN_SUIT;
        }

        if (++card_count[card_value - 1] > 4) {
            sprintf(output_buffer, "Too many '%c' cards. Excess is on line %d", buffer[0], line_number);
            set_message(output_buffer);
            fclose(file);
            return WRONG_NAME_CARD_COUNT;
        }

        ++line_number;
    }

    for (int i = 0; i < 4; ++i) {
        if (suit_count[i] != 13) {

            char *suit;
            switch (i) {
                case 0:
                    suit = "Clubs";
                    break;
                case 1:
                    suit = "Diamonds";
                    break;
                case 2:
                    suit = "Hearts";
                    break;
                case 3:
                    suit = "Spades";
                    break;
                default:
                    suit = "UNKNOWN";
            }
            sprintf(output_buffer, "Wrong number of %s cards: Should be 13 not %d", suit, suit_count[i]);
            set_message(output_buffer);
            fclose(file);
            return WRONG_SUIT_CARD_COUNT;
        }
    }

    fclose(file);
    return 0;
}

int get_user_command() {

    static char buffer[128];

    fgets(buffer, 128, stdin);
    size_t command_length = strlen(buffer);
    buffer[--command_length] = '\0';

    set_last_command(buffer);

    if (command_length == 1) {

        if (buffer[0] == 'Q') {
            // Quit current game
            return QUIT_GAME;

        } else if (buffer[0] == 'P') {
            // Start game with current deck
            return PLAY;

        } else {
            // Unknown input
            return WRONG_INPUT_FORMAT;
        }

    } else if (command_length == 2) {

        if (buffer[0] == 'Q' && buffer[1] == 'Q') {
            // Quit program
            return QUIT_PROGRAM;

        } else if (buffer[0] == 'L' && buffer[1] == 'D') {
            // Load a deck from file
            // Here, no file is specified, so a default deck should be loaded
            argument[0] = '\0';
            return LOAD_FILE;

        } else if (buffer[0] == 'S') {

            if (buffer[1] == 'W') {
                // Show all cards
                return SHOW_CARDS;

            } else if (buffer[1] == 'I') {
                // Shuffle split
                // Here, split is not specified, so a random number should be chosen
                argument[0] = '\0';
                return SHUFFLE_SPLIT;

            } else if (buffer[1] == 'R') {
                // Shuffle random
                return SHUFFLE_RANDOM;

            } else if (buffer[1] == 'D') {
                // Save cards to a file
                // Here, filename is not specified, so cards should be saved to default filename 'cards.txt'
                argument[0] = '\0';

                return SAVE_DECK;

            } else {
                return WRONG_INPUT_FORMAT;
            }
        } else {
            // Unknown input
            return WRONG_INPUT_FORMAT;
        }

    } else {

        if (buffer[0] == 'L' && buffer[1] == 'D' && buffer[2] == ' ') {
            // Load a deck from file
            // Here, a file might be specified (If there is something after the space)
            if (command_length > 3) {
                strncpy(argument, buffer + 3, command_length - 2);

            } else {
                argument[0] = '\0';
            }

            return LOAD_FILE;

        } else if (buffer[0] == 'S' && buffer[1] == 'D' && buffer[2] == ' ') {
            // Save cards to a file
            // Here, a filename might be specified (If there is something after the space)
            if (command_length > 3) {
                strncpy(argument, buffer + 3, command_length - 2);

            } else {
                argument[0] = '\0';
            }

            return SAVE_DECK;

        } else if (buffer[0] == 'S' && buffer[1] == 'I' && buffer[2] == ' ') {
            // Shuffle split
            // Here, split might be specified
            if (command_length > 3) {
                strncpy(argument, buffer + 3, command_length - 2);

            } else {
                argument[0] = '\0';
            }

            return SHUFFLE_SPLIT;

        } else {
            // Game moves
            // REGEX string if we can use it: /^[\w]{2}(:[\w]{2})?->[\w]{2}$/gm

            if (command_length == 6) {
                if (buffer[2] != '-' || buffer[3] != '>') {
                    return WRONG_INPUT_FORMAT;
                }
                source_column[0] = buffer[0];
                source_column[1] = buffer[1];

                destination_column[0] = buffer[5];
                destination_column[1] = buffer[6];

                moved_card[0] = '\0';

                return MOVE_CARD;
            } else if (command_length == 9) {
                if (buffer[2] != ':' || buffer[5] != '-' || buffer[6] != '>') {
                    return WRONG_INPUT_FORMAT;
                }
                source_column[0] = buffer[0];
                source_column[1] = buffer[1];

                moved_card[0] = buffer[3];
                moved_card[1] = buffer[4];

                destination_column[0] = buffer[7];
                destination_column[1] = buffer[8];

                return MOVE_CARD;

            } else {
                return WRONG_INPUT_FORMAT;
            }
        }
    }
}

void save_deck_to_file(linked_list *list, char *filepath) {
    FILE *file = fopen(filepath, "w");

    node *cursor = list->head;
    while (cursor != list->dummy) {
        fprintf(file, "%c%c\n", cursor->card->name, cursor->card->suit);
        cursor = cursor->next;
    }

    fclose(file);
}

char *get_moved_card() {
    return moved_card;
}

char *get_source_column() {
    return source_column;
}

char *get_destination_column() {
    return destination_column;
}

char *get_argument() {
    return argument;
}
