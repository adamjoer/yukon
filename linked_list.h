#ifndef CARD_GAME_LINKED_LIST_H
#define CARD_GAME_LINKED_LIST_H

#include <stdbool.h>

typedef struct card {
    char rank;
    char suit;
    int value;
    bool visible;
} card;

typedef struct node {
    card *card;
    struct node *prev;
    struct node *next;
} node;

typedef struct linked_list {
    node *head;
    node *dummy;
    int length;
} linked_list;

int length(linked_list *list);

void add_first(card *x, linked_list *list);

void add_last(card *x, linked_list *list);

card *remove_first(linked_list *list);

card *remove_last(linked_list *list);

card *last(linked_list *list);

node *find(const char *card, linked_list *list);

void move_card(node *card, linked_list *source, linked_list *destination);

linked_list *copy_linked_list(linked_list *list);

void shuffle_linked_list(linked_list *list);

void shuffle_array(node *array[], int length);

void free_linked_list(linked_list *list, bool free_cards);

void print_linked_list(linked_list *list);

#endif //CARD_GAME_LINKED_LIST_H
