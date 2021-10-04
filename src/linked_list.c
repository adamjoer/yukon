#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "linked_list.h"

/* Function for initialising an empty linked list with a dummy and returning a pointer to it. */
void linked_list_init(LinkedList *list) {
    if (!list)
        return;

    // Allocate space on heap for dummy node
    Node *dummy = malloc(sizeof(Node));

    // Check for error and don't go any further if no memory was allocated
    if (!dummy)
        return;

    // The dummy doesn't contain any card
    dummy->card = NULL;

    // When the list is empty, the dummy's prev and next both point back at the dummy
    dummy->prev = dummy->next = dummy;

    // Set the list's head and dummy to point at the dummy node
    list->head = list->dummy = dummy;

    // Set the list's length to 0
    list->length = 0;
}

/* Function for freeing a linked list to prevent memory leak */
void linked_list_destroy(LinkedList *list, bool free_cards) {
    if (!list)
        return;

    empty_linked_list(list, free_cards);

    free(list->dummy);
    list->head = list->dummy = NULL;
}

/*
 * Function for telling if a given linked list is empty,
 * i.e. doesn't contain any nodes other than dummy.
 */
bool is_empty(LinkedList *list) {
    if (!list)
        return true;

    return list->head == list->dummy;
}

/*
 * Function for getting the length of a linked list.
 * If the length value stored in the linked list is not valid,
 * its length will be calculated and stored in the 'length' variable.
 */
int length(LinkedList *list) {
    if (!list)
        return 0;

    if (list->length >= 0)
        return list->length;

    int length = 0;
    for (Node *cursor = list->head; cursor != list->dummy; cursor = cursor->next)
        ++length;

    list->length = length;
    return length;
}

/* Function for adding a card to the beginning of a linked list */
void add_first(Card *insert, LinkedList *list) {
    if (!list)
        return;

    // Allocate memory for the new list element
    Node *new_node = malloc(sizeof(Node));

    // If malloc returned NULL it couldn't allocate any memory; don't go any further
    if (!new_node)
        return;

    // Set the new node's card to point at the card being inserted
    new_node->card = insert;

    // Set the new node's next to point at the first element in the list
    new_node->next = list->head;

    // Set the new node's prev to point at the dummy
    new_node->prev = list->dummy;

    // Set the prev of the first element in the list to point back at the new element
    list->head->prev = new_node;

    // Set the list's head to point at the new node
    list->head = new_node;

    // Set the dummy's prev to point back at the new node
    list->dummy->next = new_node;

    // If the list's length variable is valid, increment it
    if (list->length >= 0)
        ++list->length;
}

/* Function for adding a card to the end of a linked list */
void add_last(Card *insert, LinkedList *list) {
    if (!list)
        return;

    Node *new_node = malloc(sizeof(Node));
    if (!new_node)
        return;

    new_node->card = insert;
    new_node->prev = list->dummy->prev;
    new_node->next = list->dummy;

    list->dummy->prev->next = new_node;
    list->dummy->prev = new_node;

    // If the inserted node is the only node in the list, the head needs to point at it
    if (list->head == list->dummy)
        list->head = new_node;

    // If the list's length variable is valid, increment it
    if (list->length >= 0)
        ++list->length;
}

/*
 * Function for removing the first node in a linked list
 * and returning its card
 */
Card *remove_first(LinkedList *list) {
    if (is_empty(list))
        return NULL;

    // Pointer to the node to remove
    Node *delete = list->head;

    // Set the new head, to be the value of the next element
    list->head = list->head->next;

    // Set the new head's prev pointer to the dummy
    list->head->prev = list->dummy;

    // Correct the pointer in the dummy
    list->dummy->next = list->head;

    // Save the card of the node before freeing it
    Card *card = delete->card;
    free(delete);

    // If the list's length variable is valid, decrement it
    if (list->length > 0)
        --list->length;

    // Return the card
    return card;
}

/*
 * Function for removing the last (not dummy) node in a linked list
 * and returning its card.
 */
Card *remove_last(LinkedList *list) {
    if (is_empty(list))
        return NULL;

    Node *delete = list->dummy->prev;

    delete->prev->next = list->dummy;
    list->dummy->prev = delete->prev;
    Card *card = delete->card;

    if (delete == list->head)
        list->head = list->dummy;

    free(delete);

    // If the list's length variable is valid, decrement it
    if (list->length > 0)
        --list->length;

    return card;
}

/* Function for getting the first card in a linked list */
Card *first(LinkedList *list) {
    if (!list || is_empty(list))
        return NULL;

    return list->head->card;
}

/* Function for getting the last card in a linked list */
Card *last(LinkedList *list) {
    if (!list || is_empty(list))
        return NULL;

    return list->dummy->prev->card;
}

/*
 * Function for linearly searching a linked list for a specific card
 * specified by a string. Returns null if the list doesn't contain it,
 * otherwise returns the node containing the card.
 */
Node *find_string(const char *search, LinkedList *list) {
    if (!list || strlen(search) != 2)
        return NULL;

    // Go over each node in the list
    for (Node *cursor = list->head; cursor != list->dummy; cursor = cursor->next) {

        // If the node's card matches the card being searched for, return its node
        if (cursor->card->rank == search[0] && cursor->card->suit == search[1])
            return cursor;
    }

    // The card wasn't found; return null
    return NULL;
}

/*
 * Function for linearly searching a linked list for a specific card
 * specified by a pointer. Returns false if the list doesn't contain it,
 * true otherwise.
 */
bool contains_card(Card *search, LinkedList *list) {
    if (!list || !search)
        return false;

    // Go over each node in the list
    for (Node *cursor = list->head; cursor != list->dummy; cursor = cursor->next) {

        // If the node's card matches the card being searched for, return true
        if (cursor->card == search)
            return true;
    }

    // The card wasn't found; return false
    return false;
}

/*
 * Function for linearly searching a linked list for a specific node
 * specified by a string. Returns null if the list doesn't contain it,
 * true otherwise.
 */
bool contains_node(Node *search, LinkedList *list) {
    if (!list || !search)
        return false;

    // Go over each node in the list
    for (Node *cursor = list->head; cursor != list->dummy; cursor = cursor->next) {

        // If the node's card matches the card being search for, return true
        if (cursor == search)
            return true;
    }

    // The node wasn't found; return false
    return false;
}

/*
 * Function for moving a specific node, and any potential kids of
 * it, from one linked list to another. The nodes will be added
 * to the end of the destination list.
 */
void move_node(Node *moving_node, LinkedList *source, LinkedList *destination) {
    if (!source || !destination || source == destination)
        return;

    if (!contains_node(moving_node, source))
        return;

    // Save the new last node of both lists
    Node *source_new_last = moving_node->prev;
    Node *destination_new_last = source->dummy->prev;

    // Check if the destination list is empty or not
    if (is_empty(destination)) {

        // The destination list's head is the node being moved
        destination->head = moving_node;
        destination->dummy->next = moving_node;
        moving_node->prev = destination->dummy;

    } else { // The destination list is not empty

        // The destination list's last node will point at the node being moved
        // and the moving node will point back at that
        destination->dummy->prev->next = moving_node;
        moving_node->prev = destination->dummy->prev;
    }

    // Update the pointers of the source list's dummy and new last node
    source->dummy->prev = source_new_last;
    source_new_last->next = source->dummy;

    // Update the pointers of the destination list's dummy and new last node
    destination->dummy->prev = destination_new_last;
    destination_new_last->next = destination->dummy;

    // If the node being moved was the first node in the source list, it is now empty
    if (source->head == moving_node) {

        // Set source list's head to be its dummy
        source->head = source->dummy;

        // We know the length of the source list is 0
        source->length = 0;

    } else {

        // We don't know what the length of the source list is; invalidate it
        source->length = -1;
    }

    // Invalidate the stored length of the destination list
    destination->length = -1;
}

/*
 * Function for copying a linked list, without altering it in any way.
 * This does not copy the cards in that linked list,
 * so the copy's nodes will point at the same cards as the original.
 */
void copy(LinkedList *list, LinkedList *list_copy) {
    if (!list || !list->head || !list_copy)
        return;

    for (Node *cursor = list->head; cursor != list->dummy; cursor = cursor->next)
        add_last(cursor->card, list_copy);
}

/*
 * Function for randomly shuffling the order of a linked list.
 * After shuffling, the list will have the same length,
 * and contain the same cards as it did before.
 */
void shuffle_linked_list(LinkedList *list) {
    int len = length(list);
    if (len <= 1)
        return;

    Node **node_pointers = malloc(sizeof(Node *) * len);
    if (!node_pointers)
        return;

    Node *cursor = list->head;
    for (int i = 0; i < len; ++i) {
        node_pointers[i] = cursor;
        cursor = cursor->next;
    }

    shuffle_array(node_pointers, len);

    for (int i = 0; i < len; ++i) {
        if (i == 0) {
            list->head = node_pointers[i];
            node_pointers[i]->prev = list->dummy;
            node_pointers[i]->next = node_pointers[i + 1];

            list->dummy->next = node_pointers[i];

        } else if (i == len - 1) {
            node_pointers[i]->prev = node_pointers[i - 1];
            node_pointers[i]->next = list->dummy;

            list->dummy->prev = node_pointers[i];

        } else {
            node_pointers[i]->prev = node_pointers[i - 1];
            node_pointers[i]->next = node_pointers[i + 1];
        }
    }

    free(node_pointers);
}

static void shuffle_array(Node *array[], int length) {
    if (!array)
        return;

    Node *temp;
    int random_index;
    for (int i = 0; i < length - 1; ++i) {
        random_index = i + rand() / (RAND_MAX / (length - i) + 1);
        temp = array[random_index];
        array[random_index] = array[i];
        array[i] = temp;
    }
}

/* Function for Emptying a linked list to prevent memory leak */
void empty_linked_list(LinkedList *list, bool free_cards) {
    if (!list)
        return;

    // Temporary pointer so a node being freed doesn't get lost
    Node *temp;

    // Go over each node in the linked list
    for (Node *cursor = list->head; cursor != list->dummy;) {

        // Save node in temporary pointer
        temp = cursor;

        // Set cursor to next node
        cursor = cursor->next;

        // Free node and its card
        if (free_cards)
            free(temp->card);
        free(temp);
    }

    list->head = list->dummy;
    list->dummy->prev = list->dummy->next = list->dummy;
    list->length = 0;
}

/*
 * Function for printing a linked list to console.
 * This provides extensive information about a linked list,
 * and is meant to be used for debugging purposes only.
 */
void print_linked_list(LinkedList *list) {
    if (!list) {
        printf("linked list: addr=NULL\n");
        return;
    }

    int length = 0;
    Node *cursor;
    Card *card;
    printf("linked list: addr=%p head=%p dummy=%p\n"
           "{\n", list, list->head, list->dummy);
    for (cursor = list->head; cursor != list->dummy; cursor = cursor->next) {
        card = cursor->card;

        printf("  node: prev=%p, addr=%p, next=%p\n"
               "  {\n", cursor->prev, cursor, cursor->next);
        printf("    card: addr=%p rank=%c, suit=%c, value=%d, visible=%d\n"
               "  }\n", card, card->rank, card->suit, card->value,
               card->visible);

        if (++length > 100) {
            printf("  ...\n");
            break;
        }
    }

    printf("  dummy: prev=%p, addr=%p, next=%p\n", cursor->prev, cursor, cursor->next);
    printf("  length=%d, (saved length=%d)\n", length, list->length);
    printf("}\n");
}
