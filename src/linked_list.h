#pragma once

#include <stdbool.h>

#define ASSERT_LINKED_LIST_REF(list) assert(list != NULL); assert(list->head != NULL); assert(list->dummy != NULL)
#define ASSERT_LINKED_LIST_STRUCT(list) assert(list.head != NULL); assert(list.dummy != NULL)

typedef struct Card {
    char rank;
    char suit;
    int value;
    bool visible;
} Card;

typedef struct Node {
    Card *card;
    struct Node *prev;
    struct Node *next;
} Node;

typedef struct LinkedList {
    Node *head;
    Node *dummy;
    int length;
} LinkedList;

void linked_list_init(LinkedList *list);

void linked_list_destroy(LinkedList *list, bool free_cards);

bool is_empty(LinkedList *list);

int length(LinkedList *list);

void add_first(Card *insert, LinkedList *list);

void add_last(Card *insert, LinkedList *list);

Card *remove_first(LinkedList *list);

Card *remove_last(LinkedList *list);

Card *first(LinkedList *list);

Card *last(LinkedList *list);

Node *find_string(const char *search, LinkedList *list);

bool contains_card(Card *search, LinkedList *list);

bool contains_node(Node *search, LinkedList *list);

void move_node(Node *moving_node, LinkedList *source, LinkedList *destination);

void copy(LinkedList *list, LinkedList *list_copy);

void shuffle_linked_list(LinkedList *list);

void empty_linked_list(LinkedList *list, bool free_cards);

void print_linked_list(LinkedList *list);
