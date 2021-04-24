#include <stdio.h>
#include <stdlib.h>

#include "linked_list.h"

int main() {

/*
    linked_list *list = malloc(sizeof(linked_list));

    list->head = NULL;
    list->dummy = NULL;

    card *foo = malloc(sizeof(card));
    foo->name = 'K';
    foo->suit = 'H';
    foo->value = 13;
    foo->visible = true;
    add_last(foo, list);

    foo = malloc(sizeof(card));
    foo->name = 'K';
    foo->suit = 'S';
    foo->value = 11;
    foo->visible = true;
    add_last(foo, list);

    foo = malloc(sizeof(card));
    foo->name = 'T';
    foo->suit = 'C';
    foo->value = 10;
    foo->visible = false;
    add_last(foo, list);

    foo = malloc(sizeof(card));
    foo->name = '2';
    foo->suit = 'D';
    foo->value = 2;
    foo->visible = false;
    add_last(foo, list);

    print_linked_list(list);

    free_linked_list(list);
*/


    linked_list *list1 = malloc(sizeof(linked_list));
    list1->head = list1->dummy = NULL;

    linked_list *list2 = malloc(sizeof(linked_list));
    list2->head = list2->dummy = NULL;

    card *foo = malloc(sizeof(card));
    foo->name = 'K';
    foo->suit = 'H';
    foo->value = 13;
    foo->visible = true;
    add_last(foo, list1);

    foo = malloc(sizeof(card));
    foo->name = 'K';
    foo->suit = 'S';
    foo->value = 11;
    foo->visible = true;
    add_last(foo, list1);

    foo = malloc(sizeof(card));
    foo->name = 'T';
    foo->suit = 'C';
    foo->value = 10;
    foo->visible = false;
    add_last(foo, list2);

    foo = malloc(sizeof(card));
    foo->name = '2';
    foo->suit = 'H';
    foo->value = 2;
    foo->visible = false;
    add_last(foo, list2);

    printf("Before:\nList 1:\n");
    print_linked_list(list1);
    printf("List 2:\n");
    print_linked_list(list2);

    move_card("KS", list1, list2);

    printf("\nList 1:\n");
    print_linked_list(list1);
    printf("List 2:\n");
    print_linked_list(list2);

    move_card("TC", list2, list1);

    printf("\nAfter:\nList 1:\n");
    print_linked_list(list1);

    printf("List 2:\n");
    print_linked_list(list2);

    printf("\n");
    for (int i = 0; i < 4; ++i) {
        printf("Removing #%d...\n", i + 1);
        free(remove_last(list1));
//        print_linked_list(list1);
    }

    print_linked_list(list1);

    free_linked_list(list1);
    free_linked_list(list2);

    return 0;
}
