#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "linked_list.h"

/*
 * Function for getting the length of a linked list.
 * If the length value stored in the linked list is not valid,
 * its length will be calculated and stored in the 'length' variable.
 */
int length(linked_list *list) {
    if (!list || !list->head)
        return 0;

    if (list->length >= 0)
        return list->length;

    node *cursor = list->head;

    int length = 0;
    while (cursor != list->dummy) {
        ++length;
        cursor = cursor->next;
    }

    list->length = length;
    return length;
}

/* Function for adding a card to the beginning of a linked list */
void add_first(card *x, linked_list *list) {

    node **head = &list->head;
    node **dummy = &list->dummy;

    // Allocate memory for the new list element
    node *insert = malloc(sizeof(node));
    if (!insert) {

        // If malloc returned NULL it couldn't allocate any space, stop the program
        perror("add_first");
        exit(1);
    }

    // Initialize values in the node
    insert->card = x;
    insert->next = *head;

    // If the list isn't empty, the first element's prev needs to point at the insert
    if (*head)
        (*head)->prev = insert;

    // Change head so it points at the new node
    *head = insert;

    if (!*dummy) {

        // If dummy is NULL, it needs to be initialized
        *dummy = malloc(sizeof(node));
        if (!*dummy) {

            // Again check if malloc failed
            perror("add_first");
            exit(1);
        }
        (*dummy)->card = NULL;
        (*dummy)->prev = *head;

        // The last element in the list should point at dummy.
        // Since there is only one element in it right now, the first element should point at dummy
        insert->next = *dummy;
    }

    // Change dummy's pointers so that it points that he new start
    (*dummy)->next = *head;

    // The first element's prev should point at dummy;
    insert->prev = *dummy;

    if (list->length >= 0)
        ++list->length;
}

/* Function for adding a card to the end of a linked list */
void add_last(card *x, linked_list *list) {

    node **head = &list->head;
    node **dummy = &list->dummy;

    // Allocate memory for the new list element
    node *insert = malloc(sizeof(node));
    if (!insert) {

        // If malloc returned NULL it couldn't allocate any space, stop the program
        perror("add_last");
        exit(1);
    }
    insert->card = x;

    if (!*dummy) {

        *head = insert;

        // If dummy is NULL, it needs to be initialized
        *dummy = malloc(sizeof(node));
        if (!*dummy) {

            // Again check if malloc failed
            perror("add_last");
            exit(1);
        }
        (*dummy)->card = NULL;
        (*dummy)->next = (*dummy)->prev = *head;

        // The last element in the list should point at dummy.
        // Since there is only one element in it right now, the first element should point at dummy
        insert->next = insert->prev = *dummy;

    } else {
        insert->prev = (*dummy)->prev;
        insert->next = *dummy;

        ((*dummy)->prev)->next = insert;
        (*dummy)->prev = insert;
    }

    if (list->length >= 0)
        ++list->length;
}

/* Function for removing the first node in a linked list and returning its card */
card *remove_first(linked_list *list) {
    node **head = &list->head;
    node **dummy = &list->dummy;

    if (!*head) {
        printf("Cannot remove node from empty list\n");
        exit(1);
    }

    // Pointer to the node to remove
    node *delete = *head;

    // Set the new head, to be the value of the next element
    *head = (*head)->next;

    // Set the new head's prev pointer to the dummy
    (*head)->prev = *dummy;

    // Correct the pointer in the dummy
    (*dummy)->next = *head;

    // Save the card of the node before freeing it
    card *card = delete->card;
    free(delete);

    // If the list is now empty, dummy should also be freed
    if (*head == *dummy) {
        free(*dummy);
        *head = *dummy = NULL;
    }

    if (list->length > 0)
        --list->length;

    // Return the card
    return card;
}

/* Function for removing the last (not dummy) node in a linked list and returning its card */
card *remove_last(linked_list *list) {
    node **head = &list->head;
    node **dummy = &list->dummy;

    if (!*head) {
        printf("Cannot remove node from empty list\n");
        exit(1);
    }

    node *delete = (*dummy)->prev;

    if (delete == *head) {
        free(*dummy);
        *head = *dummy = NULL;

    } else {
        (delete->prev)->next = *dummy;
        (*dummy)->prev = delete->prev;
    }
    card *card = delete->card;

    free(delete);

    if (list->length > 0)
        --list->length;

    return card;
}

/* Function for getting the last card in a linked list */
card *last(linked_list *list) {
    if (!list || !list->dummy)
        return NULL;

    return list->dummy->prev->card;
}

/*
 * Function for linearly searching a linked list for a specific card.
 * Returns null if the list doesn't contain it.
 */
node *find_string(const char *search, linked_list *list) {
    if (strlen(search) != 2)
        return NULL;

    // Pointer for going over the list
    node *cursor = list->head;

    // Go over each node in the list
    while (cursor != list->dummy) {

        // If the node's card matches the card being searched for, return its node
        if (cursor->card->rank == search[0] && cursor->card->suit == search[1])
            return cursor;

        // Set cursor to next node
        cursor = cursor->next;
    }

    // The card wasn't found; return null
    return NULL;
}

bool contains_card(card *search, linked_list *list) {

    // Pointer for going over the list
    node *cursor = list->head;

    // Go over each node in the list
    while (cursor != list->dummy) {

        // If the node's card matches the card being searched for, return its node
        if (cursor->card == search)
            return true;

        // Set cursor to next node
        cursor = cursor->next;
    }

    // The card wasn't found; return null
    return false;
}

bool contains_node(node *search, linked_list *list) {

    // Pointer for going over the list
    node *cursor = list->head;

    // Go over each node in the list
    while (cursor != list->dummy) {

        // If the node's card matches the card being search for, return its node
        if (cursor == search)
            return true;

        // Set cursor to next node
        cursor = cursor->next;
    }

    // The node wasn't found; return null
    return false;

}

/*
 * Function for moving a given node, and any potential nodes after it, from one linked list to another.
 * The nodes will be added to the end of the destination list.
 */
void move_node(node *card, linked_list *origin, linked_list *destination) {
    if (origin == destination)
        return;

    if (!contains_node(card, origin))
        return;

    // Pointers to make the code (a bit) less fugly
    node **source_head = &origin->head;
    node **source_dummy = &origin->dummy;

    node **destination_head = &destination->head;
    node **destination_dummy = &destination->dummy;

    // Save the new last node of both lists
    node *source_new_last = card->prev;
    node *destination_new_last = (*source_dummy)->prev;

    // Check if the destination list is empty or not
    if (!*destination_head) {

        // If the destination list is empty, a new dummy needs to be made
        *destination_dummy = malloc(sizeof(node));
        if (!*destination_dummy) {
            perror("move_node");
            exit(1);
        }

        // Initialize dummy values
        (*destination_dummy)->card = NULL;

        // The destination list's head is the node being moved
        (*destination_dummy)->next = card;
        *destination_head = card;
        card->prev = *destination_dummy;

    } else { // The destination list is not empty

        // The destination list's last node will point at the node being moved and the moved node will point back at that
        ((*destination_dummy)->prev)->next = card;
        card->prev = (*destination_dummy)->prev;
    }

    // Update the pointers of the origin list's dummy and new last node
    (*source_dummy)->prev = source_new_last;
    source_new_last->next = *source_dummy;

    // Update the pointers of the destination list's dummy and new last node
    (*destination_dummy)->prev = destination_new_last;
    destination_new_last->next = *destination_dummy;

    // If the node being moved was the first node in the origin list, it is now empty
    if (*source_head == card) {

        // Free dummy and set the head and dummy to null
        free(*source_dummy);
        *source_head = *source_dummy = NULL;
    }

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

    linked_list *list_copy = malloc(sizeof(linked_list));
    if (!list_copy) {
        perror("copy");
        exit(1);
    }

    list_copy->dummy = malloc(sizeof(node));
    if (!list_copy->dummy) {
        perror("copy");
        exit(1);
    }

    list_copy->dummy->card = NULL;

    node *copy_cursor = list->head;
    node *cur_node = NULL;
    node *prev_node;
    for (int i = 1, n = length(list); i <= n; ++i) {
        prev_node = cur_node;

        cur_node = malloc(sizeof(node));
        if (!cur_node) {
            perror("copy");
            exit(1);
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
    if (!node_pointers) {
        perror("shuffle_linked_list");
        exit(1);
    }

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

void shuffle_array(node *array[], int length) {
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

    // Pointer for going over the list
    node *cursor = list->head;

    // Temporary pointer so a node being freed doesn't get lost
    node *temp;

    // Go over each node in the linked list
    while (cursor != list->dummy) {

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

void print_linked_list(linked_list *list) {
    if (!list) {
        printf("{\n  NULL\n}\n");
        return;
    }

    if (!list->head) {
        printf("{\n  EMPTY\n}\n");
        return;
    }

    int length = 0;
    node *cursor = list->head;
    card *card;
    printf("{\n");
    printf("  head=%p\n", list->head);
    while (cursor != list->dummy) {

        card = cursor->card;

        printf("  node: prev=%p, addr=%p, next=%p\n", cursor->prev, cursor, cursor->next);
        printf("    card: rank=%c, suit=%c, value=%d, visible=%d\n", card->rank, card->suit, card->value,
               card->visible);

        cursor = cursor->next;

        if (++length > 100) {
            printf("  ...\n");
            break;
        }
    }

    printf("  dummy: prev=%p, addr=%p, next=%p\n", cursor->prev, cursor, cursor->next);
    printf("  length=%d, (saved length=%d)\n", length, list->length);
    printf("}\n");
}
