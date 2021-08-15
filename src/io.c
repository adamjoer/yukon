#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#include "io.h"
#include "gui.h"

char moved_card[3];
char source_column[3];
char destination_column[3];
char argument[IN_BUFFER_SIZE];

linked_list *load_from_file(char *filepath, bool check_file) {
    if (check_file) {
        if (validate_file(filepath) != OK)
            return NULL;
    }

    FILE *file = fopen(filepath, "r");

    linked_list *list = init_linked_list();

    char rank, suit;
    card *new_card;

    while (fscanf(file, "%c%c\n", &rank, &suit) != EOF) {
        new_card = malloc(sizeof(card));
        if (!new_card) {
            free_linked_list(list, false);
            return NULL;
        }

        new_card->rank = rank;
        new_card->suit = suit;
        new_card->value = get_card_value(rank);
        new_card->visible = false;

        add_last(new_card, list);
    }

    fclose(file);

    return list;
}

enum validation_status validate_file(char *filepath) {

    char output_buffer[MESSAGE_BUFFER_SIZE];

    FILE *file = fopen(filepath, "r");
    if (!file) {
        sprintf(output_buffer, "File '%s' couldn't be opened: %s", filepath, strerror(errno));
        set_message(output_buffer);
        return FILE_NOT_FOUND;
    }

    int card_value;

    int suit_count[4];
    for (int i = 0; i < 4; ++i)
        suit_count[i] = 0;

    int card_count[13];
    for (int i = 0; i < 13; ++i)
        card_count[i] = 0;

    int line_number = 1;
    char line_buffer[IN_BUFFER_SIZE];

    /*
     * NOTE: The number in the format string (63) is IN_BUFFER_SIZE - 1.
     *       This prevents that lines that are longer than the line buffer
     *       causes buffer overflow.
     */
    while (fscanf(file, "%63[^\r\n] ", line_buffer) != EOF) {

        if (strlen(line_buffer) != 2) {
            sprintf(output_buffer,
                    "Unknown card format '%s' on line %d: "
                    "Valid format is [rank-char][suit-char] e.g. TH for ten of hearts",
                    line_buffer, line_number);
            set_message(output_buffer);

            fclose(file);
            return INVALID_FORMAT;
        }

        card_value = get_card_value(line_buffer[0]);
        if (card_value == -1) {
            sprintf(output_buffer,
                    "Unknown card rank '%c' on line %d: "
                    "Valid ranks are A, 2-9, T, J, Q, K",
                    line_buffer[0], line_number);
            set_message(output_buffer);

            fclose(file);
            return INVALID_RANK;
        }

        switch (line_buffer[1]) {
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
                sprintf(output_buffer,
                        "Unknown card suit '%c' on line %d: "
                        "Valid suits are C, D, H, S",
                        line_buffer[1], line_number);
                set_message(output_buffer);

                fclose(file);
                return INVALID_SUIT;
        }

        if (++card_count[card_value - 1] > 4) {
            sprintf(output_buffer, "Too many '%c' cards. Excess is on line %d",
                    line_buffer[0], line_number);
            set_message(output_buffer);

            fclose(file);
            return INVALID_RANK_CARD_COUNT;
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
            return INVALID_SUIT_CARD_COUNT;
        }
    }

    fclose(file);
    return OK;
}

enum command get_user_command() {

    source_column[2] = destination_column[2] = moved_card[2] = '\0';

    char input_buffer[IN_BUFFER_SIZE];

    if (!fgets(input_buffer, IN_BUFFER_SIZE, stdin))
        return ERROR;

    size_t input_length = strlen(input_buffer);
    if (input_buffer[input_length - 1] == '\n')
        input_buffer[--input_length] = '\0';

    set_last_command(input_buffer);

    input_buffer[0] = (char) toupper(input_buffer[0]);
    input_buffer[1] = (char) toupper(input_buffer[1]);

    if (input_length == 1) {

        if (input_buffer[0] == 'Q') {
            // Quit current game
            return QUIT_GAME;

        } else if (input_buffer[0] == 'P') {
            // Start game with current deck
            return PLAY;

        } else {
            // Unknown input
            return INVALID_INPUT_FORMAT;
        }

    } else if (input_length == 2) {

        if (input_buffer[0] == 'Q' && input_buffer[1] == 'Q') {
            // Quit program
            return QUIT_PROGRAM;

        } else if (input_buffer[0] == 'L' && input_buffer[1] == 'D') {
            // Load a deck from file
            // Here, no file is specified, so a default deck should be loaded
            argument[0] = '\0';
            return LOAD_FILE;

        } else if (input_buffer[0] == 'S') {

            if (input_buffer[1] == 'W') {
                // Show all cards
                return SHOW_CARDS;

            } else if (input_buffer[1] == 'I') {
                // Shuffle split
                // Here, split is not specified, so a random number should be chosen
                argument[0] = '\0';
                return SHUFFLE_SPLIT;

            } else if (input_buffer[1] == 'R') {
                // Shuffle random
                return SHUFFLE_RANDOM;

            } else if (input_buffer[1] == 'D') {
                // Save cards to a file
                // Here, filename is not specified, so cards should be saved to default filename 'cards.txt'
                argument[0] = '\0';

                return SAVE_DECK;

            } else {
                return INVALID_INPUT_FORMAT;
            }
        } else {
            // Unknown input
            return INVALID_INPUT_FORMAT;
        }

    } else {

        if (input_buffer[0] == 'L' && input_buffer[1] == 'D' && input_buffer[2] == ' ') {
            // Load a deck from file

            // Here, a file might be specified (If there is something after the space)
            if (input_length > 3)
                strncpy(argument, input_buffer + 3, input_length - 2);
            else
                argument[0] = '\0';

            return LOAD_FILE;

        } else if (input_buffer[0] == 'S' && input_buffer[1] == 'D' && input_buffer[2] == ' ') {
            // Save cards to a file

            // Here, a filename might be specified (If there is something after the space)
            if (input_length > 3)
                strncpy(argument, input_buffer + 3, input_length - 2);
            else
                argument[0] = '\0';

            return SAVE_DECK;

        } else if (input_buffer[0] == 'S' && input_buffer[1] == 'I' && input_buffer[2] == ' ') {
            // Shuffle split
            // Here, split might be specified
            if (input_length > 3)
                strncpy(argument, input_buffer + 3, input_length - 2);

            else
                argument[0] = '\0';

            return SHUFFLE_SPLIT;

        } else {
            // Game moves
            // REGEX string if we can use it: /^[\w]{2}(:[\w]{2})?->[\w]{2}$/gm

            if (input_length == 6) {
                if (input_buffer[2] != '-' || input_buffer[3] != '>')
                    return INVALID_INPUT_FORMAT;

                source_column[0] = input_buffer[0];
                source_column[1] = input_buffer[1];

                destination_column[0] = (char) toupper(input_buffer[4]);
                destination_column[1] = (char) toupper(input_buffer[5]);

                moved_card[0] = '\0';

                return MOVE_CARD;

            } else if (input_length == 9) {
                if (input_buffer[2] != ':' || input_buffer[5] != '-' || input_buffer[6] != '>')
                    return INVALID_INPUT_FORMAT;

                source_column[0] = input_buffer[0];
                source_column[1] = input_buffer[1];

                moved_card[0] = (char) toupper(input_buffer[3]);
                moved_card[1] = (char) toupper(input_buffer[4]);

                destination_column[0] = (char) toupper(input_buffer[7]);
                destination_column[1] = (char) toupper(input_buffer[8]);

                return MOVE_CARD;

            } else {
                return INVALID_INPUT_FORMAT;
            }
        }
    }
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

bool is_valid_column(char *string) {
    return strlen(string) == 2 && string[0] == 'C' && isdigit(string[1]) &&
           string[1] >= '1' && string[1] <= NO_COLUMNS + '0';
}

bool is_valid_foundation(char *string) {
    return strlen(string) == 2 && string[0] == 'F' && isdigit(string[1]) &&
           string[1] >= '1' && string[1] <= NO_FOUNDATIONS + '0';
}

bool is_valid_card(char *string) {
    return strlen(string) == 2 && get_card_value(string[0]) != -1 &&
           (string[1] == 'C' || string[1] == 'D' || string[1] == 'H' || string[1] == 'S');
}

void save_deck_to_file(linked_list *list, char *filepath) {
    char output_buffer[MESSAGE_BUFFER_SIZE];

    FILE *file = fopen(filepath, "w");
    if (!file) {
        sprintf(output_buffer, "File '%s' not created: %s", filepath, strerror(errno));
        set_message(output_buffer);
        return;
    }

    for (node *cursor = list->head; cursor != list->dummy; cursor = cursor->next)
        fprintf(file, "%c%c\n", cursor->card->rank, cursor->card->suit);

    fclose(file);
    sprintf(output_buffer, "Deck saved to file '%s'", filepath);
    set_message(output_buffer);
}