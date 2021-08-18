#pragma once

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

void init_linked_list(linked_list *list);

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

void move_node(node *moving_node, linked_list *source, linked_list *destination);

void copy(linked_list *list, linked_list *list_copy);

void shuffle_linked_list(linked_list *list);

static void shuffle_array(node *array[], int length);

void empty_linked_list(linked_list *list, bool free_cards);

void free_linked_list(linked_list *list, bool free_cards);

void print_linked_list(linked_list *list);
