#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    int clubs_count = 0, diamonds_count = 0, hearts_count = 0, spades_count = 0;
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
                clubs_count++;
                break;
            case 'D':
                diamonds_count++;
                break;
            case 'H':
                hearts_count++;
                break;
            case 'S':
                spades_count++;
                break;
            default:
                printf("Unknown card suit '%c' on line %d: Valid suits are C, D, H, S\n", buffer[1], line_number);
                fclose(file);
                return UNKNOWN_SUIT;
        }

        if (++card_count[card_value - 1] > 4) {
            printf("Too many '%c' cards. Excess is on line %d\n", get_card_name(card_value), line_number);
            fclose(file);
            return WRONG_NAME_CARD_COUNT;
        }

        line_number++;
    }

    if (clubs_count != 13) {
        printf("Wrong number of clubs cards: Should be 13 not %d\n", clubs_count);
        fclose(file);
        return WRONG_SUIT_CARD_COUNT;
    }

    if (diamonds_count != 13) {
        printf("Wrong number of diamonds cards: Should be 13 not %d\n", diamonds_count);
        fclose(file);
        return WRONG_SUIT_CARD_COUNT;
    }

    if (hearts_count != 13) {
        printf("Wrong number of hearts cards: Should be 13 not %d\n", hearts_count);
        fclose(file);
        return WRONG_SUIT_CARD_COUNT;
    }

    if (spades_count != 13) {
        printf("Wrong number of spades cards: Should be 13 not %d\n", spades_count);
        fclose(file);
        return WRONG_SUIT_CARD_COUNT;
    }

    fclose(file);

    return 0;
}