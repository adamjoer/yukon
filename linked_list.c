#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "linked_list.h"

/*
 * Function for initialising an empty linked list with a dummy and returning a pointer to it.
 */
linked_list *init_linked_list() {
    linked_list *list = malloc(sizeof(linked_list));
    if (!list) {
        return NULL;
    }

    node *dummy = malloc(sizeof(node));
    if (!dummy) {
        free(list);
        return NULL;
    }

    dummy->card = NULL;
    dummy->prev = dummy->next = dummy;

    list->head = list->dummy = dummy;
    list->length = 0;

    return list;
}

bool is_empty(linked_list *list) {
    if (!list)
        return true;

    return list->head == list->dummy;
}

/*
 * Function for getting the length of a linked list.
 * If the length value stored in the linked list is not valid,
 * its length will be calculated and stored in the 'length' variable.
 */
int length(linked_list *list) {
    if (!list)
        return 0;

    if (list->length >= 0)
        return list->length;

    int length = 0;
    for (node *cursor = list->head; cursor != list->dummy; cursor = cursor->next)
        ++length;

    list->length = length;
    return length;
}

/* Function for adding a card to the beginning of a linked list */
void add_first(card *insert, linked_list *list) {

    // Allocate memory for the new list element
    node *new_node = malloc(sizeof(node));

    // If malloc returned NULL it couldn't allocate any memory; don't go further
    if (!new_node)
        return;

    // Initialize values in the node
    new_node->card = insert;
    new_node->next = list->head;
    new_node->prev = list->dummy;

    list->head->prev = new_node;
    list->head = new_node;
    list->dummy->next = new_node;

    if (list->length >= 0)
        ++list->length;
}

/* Function for adding a card to the end of a linked list */
void add_last(card *insert, linked_list *list) {

    // Allocate memory for the new list element
    node *new_node = malloc(sizeof(node));

    // If malloc returned NULL it couldn't allocate any memory; don't go further
    if (!new_node)
        return;

    new_node->card = insert;

    if (is_empty(list)) {
        new_node->prev = list->dummy;
        new_node->next = list->dummy;

        list->dummy->next = new_node;
        list->dummy->prev = new_node;

        list->head = new_node;

    } else {
        new_node->prev = list->dummy->prev;
        new_node->next = list->dummy;

        list->dummy->prev->next = new_node;
        list->dummy->prev = new_node;
    }

    if (list->length >= 0)
        ++list->length;
}

/*
 * Function for removing the first node in a linked list
 * and returning its card
 */
card *remove_first(linked_list *list) {
    if (is_empty(list))
        return NULL;

    // Pointer to the node to remove
    node *delete = list->head;

    // Set the new head, to be the value of the next element
    list->head = list->head->next;

    // Set the new head's prev pointer to the dummy
    list->head->prev = list->dummy;

    // Correct the pointer in the dummy
    list->dummy->next = list->head;

    // Save the card of the node before freeing it
    card *card = delete->card;
    free(delete);

    if (list->length > 0)
        --list->length;

    // Return the card
    return card;
}

/*
 * Function for removing the last (not dummy) node in a linked list
 * and returning its card.
 */
card *remove_last(linked_list *list) {
    if (is_empty(list))
        return NULL;

    node *delete = list->dummy->prev;

    delete->prev->next = list->dummy;
    list->dummy->prev = delete->prev;
    card *card = delete->card;

    if (delete == list->head)
        list->head = list->dummy;

    free(delete);

    if (list->length > 0)
        --list->length;

    return card;
}

/* Function for getting the last card in a linked list */
card *last(linked_list *list) {
    if (!list || is_empty(list))
        return NULL;

    return list->dummy->prev->card;
}

/*
 * Function for linearly searching a linked list for a specific card
 * specified by a string. Returns null if the list doesn't contain it,
 * otherwise returns the node containing the card.
 */
node *find_string(const char *search, linked_list *list) {
    if (strlen(search) != 2)
        return NULL;

    // Go over each node in the list
    for (node *cursor = list->head; cursor != list->dummy; cursor = cursor->next) {

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
bool contains_card(card *search, linked_list *list) {

    // Go over each node in the list
    for (node *cursor = list->head; cursor != list->dummy; cursor = cursor->next) {

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
bool contains_node(node *search, linked_list *list) {

    // Go over each node in the list
    for (node *cursor = list->head; cursor != list->dummy; cursor = cursor->next) {

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
void move_node(node *moving_node, linked_list *origin, linked_list *destination) {
    if (origin == destination)
        return;

    if (!contains_node(moving_node, origin))
        return;

    // Save the new last node of both lists
    node *origin_new_last = moving_node->prev;
    node *destination_new_last = origin->dummy->prev;

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

    // Update the pointers of the origin list's dummy and new last node
    origin->dummy->prev = origin_new_last;
    origin_new_last->next = origin->dummy;

    // Update the pointers of the destination list's dummy and new last node
    destination->dummy->prev = destination_new_last;
    destination_new_last->next = destination->dummy;

    // If the node being moved was the first node in the origin list, it is now empty
    if (origin->head == moving_node) {

        // Set origin list's head to be its dummy
        origin->head = origin->dummy;
    }

    // Invalidate the stored length of both lists
    origin->length = destination->length = -1;
}

/*
 * Function for copying a linked list, without altering it in any way
 * This does not copy the cards in that linked list,
 * so the copy's nodes will point at the same cards as the original.
 */
linked_list *copy(linked_list *list) {
    if (!list)
        return NULL;

    linked_list *list_copy = init_linked_list();
    if (!list_copy)
        return NULL;

    node *copy_cursor = list->head;
    node *cur_node = NULL;
    node *prev_node;
    for (int i = 1, n = length(list); i <= n; ++i) {
        prev_node = cur_node;

        cur_node = malloc(sizeof(node));
        if (!cur_node) {
            free_linked_list(list_copy, false);
            return NULL;
        }

        cur_node->card = copy_cursor->card;

        if (i == 1) {
            list_copy->head = cur_node;
            list_copy->dummy->next = cur_node;
            cur_node->prev = list_copy->dummy;

        } else if (i == n) {
            prev_node->next = cur_node;
            cur_node->prev = prev_node;
            cur_node->next = list_copy->dummy;
            list_copy->dummy->prev = cur_node;

        } else {
            cur_node->prev = prev_node;
            prev_node->next = cur_node;
        }

        copy_cursor = copy_cursor->next;
    }

    list_copy->length = list->length;
    return list_copy;
}

/*
 * Function for randomly shuffling the order of a linked list.
 * After shuffling, the list will have the same length,
 * and contain the same cards as it did before.
 */
void shuffle_linked_list(linked_list *list) {
    int len = length(list);
    if (len <= 1)
        return;

    node **node_pointers = malloc(sizeof(node *) * len);
    if (!node_pointers)
        return;

    node *cursor = list->head;
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

static void shuffle_array(node *array[], int length) {
    srand(time(NULL));

    node *temp;
    int random_index;
    for (int i = 0; i < length - 1; ++i) {
        random_index = i + rand() / (RAND_MAX / (length - i) + 1);
        temp = array[random_index];
        array[random_index] = array[i];
        array[i] = temp;
    }
}

/* Function for freeing a linked list to prevent memory leak */
void free_linked_list(linked_list *list, bool free_cards) {
    if (!list)
        return;

    // Temporary pointer so a node being freed doesn't get lost
    node *temp;

    // Go over each node in the linked list
    for (node *cursor = list->head; cursor != list->dummy;) {

        // Save node in temporary pointer
        temp = cursor;

        // Set cursor to next node
        cursor = cursor->next;

        // Free node and its card
        if (free_cards)
            free(temp->card);
        free(temp);
    }

    // Free the dummy and the list struct
    free(list->dummy);
    free(list);
}

/*
 * Function for printing a linked list to console.
 * This is provides extensive information about a linked list,
 * and is meant to be used for debugging purposes only.
 */
void print_linked_list(linked_list *list) {
    if (!list) {
        printf("linked list: addr=NULL\n");
        return;
    }

    int length = 0;
    node *cursor;
    card *card;
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
