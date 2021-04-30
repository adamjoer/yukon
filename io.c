#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <regex.h>

#include "io.h"
#include "game.h"

linked_list *load_from_file(char *filepath) {
    if (validate_file(filepath) != 0) {
        return NULL;
    }

    FILE *file = fopen(filepath, "r");

    linked_list *list = malloc(sizeof(linked_list));
    list->head = list->dummy = NULL;

    char buffer[2];

    card *new_card;
    while (fscanf(file, "%s\n", buffer) != EOF) {
        new_card = malloc(sizeof(card));
        new_card->name = buffer[0];
        new_card->suit = buffer[1];
        new_card->value = get_card_value(buffer[0]);
        new_card->visible = false;

        add_last(new_card, list);
    }

    fclose(file);

    return list;
}

int validate_file(char *filepath) {

    FILE *file = fopen(filepath, "r");
    if (!file) {
        printf("File '%s' not found\n", filepath);
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

    while (fscanf(file, "%s\r\n", buffer) != EOF) {
        if (strlen(buffer) != 2) {
            printf("Unknown card format '%s' on line %d: Valid format is [name-char][suit-char] e.g. TH for ten of hearts\n",
                   buffer, line_number);
            fclose(file);
            return UNKNOWN_FORMAT;
        }

        card_value = get_card_value(buffer[0]);
        if (card_value == -1) {
            printf("Unknown card name '%c' on line %d: Valid names are A, 2-9, T, J, Q, K\n", buffer[0], line_number);
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
                printf("Unknown card suit '%c' on line %d: Valid suits are C, D, H, S\n", buffer[1], line_number);
                fclose(file);
                return UNKNOWN_SUIT;
        }

        if (++card_count[card_value - 1] > 4) {
            printf("Too many '%c' cards. Excess is on line %d\n", buffer[0], line_number);
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
            printf("Wrong number of %s cards: Should be 13 not %d\n", suit, suit_count[i]);

            fclose(file);
            return WRONG_SUIT_CARD_COUNT;
        }
    }

    fclose(file);
    return 0;
}

int get_user_input() {

    char buffer[128];

    scanf("%s", buffer);

    if (strlen(buffer) == 1) {
        if (buffer[0] == 'Q') {
            // Quit current game

        } else if (buffer[0] == 'P') {
            // Start game with current deck

        } else {
            // Unknown input
        }

    } else {

        if (buffer[0] == 'Q' && buffer[1] == 'Q') {
            // Quit program

        } else if (buffer[0] == 'L' && buffer[1] == 'D') {
            // Load a deck of cards from file <filename>
            // If filename is not specified, load a sorted deck by default

        } else if (buffer[0] == 'S' && buffer[1] == 'W') {
            // Show all cards

        } else if (buffer[0] == 'S' && buffer[1] == 'I') {
            // Shuffle split

        } else if (buffer[0] == 'S' && buffer[1] == 'R') {
            // Shuffle random

        } else if (buffer[0] == 'S' && buffer[1] == 'D') {
            // Save cards to a file name <filename>
            // If <filename> is not specified, write to file named 'cards.txt'

        } else {
            // Game moves
            // REGEX string if we can use it: /^[\w]{2}(:[\w]{2})?->[\w]{2}$/gm


        }
    }

    return 0;
}
