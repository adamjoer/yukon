#pragma once

#include <stdbool.h>

typedef struct {
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

typedef struct {
    node *head;
    node *dummy;
    int length;
} linked_list;

linked_list *init_linked_list();

bool is_empty(linked_list *list);

int length(linked_list *list);

void add_first(card *insert, linked_list *list);

void add_last(card *insert, linked_list *list);

card *remove_first(linked_list *list);

card *remove_last(linked_list *list);

card *first(linked_list *list);

card *last(linked_list *list);

node *find_string(const char *search, linked_list *list);

bool contains_card(card *search, linked_list *list);

bool contains_node(node *search, linked_list *list);

void move_node(node *moving_node, linked_list *origin, linked_list *destination);

linked_list *copy(linked_list *list);

void shuffle_linked_list(linked_list *list);

static void shuffle_array(node *array[], int length);

void free_linked_list(linked_list *list, bool free_cards);

void print_linked_list(linked_list *list);
