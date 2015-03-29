/* Nom: linked_list.c
 * Description: module qui gère les liste chaine
 * Date: 22.03.2015
 * version : 1.0
 * responsable du module : Alexandre Devienne
 * groupe : Alexandre Devienne, Pauline Maury Laribière
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "linked_list.h"


//static void sort_left(LIST_HEAD *head);
static void swap_data(LIST_ELEMENT *p_a, LIST_ELEMENT *p_b);


LIST_HEAD list_create(void (*deleteData) (void *data), int (*sortData) (void *a, void *b)) {
    LIST_HEAD head;

    head.first   = NULL;
    head.current = NULL;
    head.last    = NULL;

    head.nbElements = 0;

    head.deleteData = deleteData;
    head.sortData = sortData;
    
    return head;
}


LIST_ELEMENT* list_goToFirst(LIST_HEAD *head) {
    LIST_ELEMENT *p = NULL;

    if (head != NULL) {
        head->current = head->first;
        p = head->current;
    }

    return p;
}

LIST_ELEMENT* list_goToNext (LIST_HEAD *head) {
    LIST_ELEMENT *p = NULL;

    if (head != NULL) {
        if (head->current != NULL) {
            head->current = head->current->next;
        } else {
            // what to do ? if current == NULL
            ;
        }

        p = head->current;
    }

    return p;
}

void list_fctToAllNext(LIST_HEAD *head, void (*func) (void *data)) {
    if (head!=NULL && head->current!=NULL && func!=NULL) {
        do {
            (*func)(head->current->data);
        } while (list_goToNext(head) != NULL);
    }
}

void list_fctToAllElements(LIST_HEAD *head, void (*func) (void *data)) {
    if (head != NULL) {
        list_goToFirst(head);
        list_fctToAllNext(head, func);
    }
}


LIST_ELEMENT* list_add(LIST_HEAD *head, void *data) {
    LIST_ELEMENT *p = NULL;

    if (head != NULL) {
        p = malloc(sizeof(LIST_ELEMENT));

        if (p == NULL) {
            printf("Memory allocation failure in %s\n", __func__);
            exit(EXIT_FAILURE);
        }

        if (head->last == NULL) { // list empty (then first also == NULL)
            head->first = p;
            p->prev = NULL;
        } else {
            head->last->next = p;
            p->prev = head->last;

        }
        
        p->data = data;
        p->next = NULL;
        head->last = p;

        head->nbElements++;
    }

    return p;
}


// return new current
LIST_ELEMENT* list_deleteCurrent(LIST_HEAD *head) {
    LIST_ELEMENT *prev;
    LIST_ELEMENT *next;

    if (head!=NULL && head->current!=NULL) {
        prev = head->current->prev;
        next = head->current->next;

        if (prev != NULL) {
            prev->next = next;
        } else {
            head->first = next;
        }

        if (next != NULL) {
            next->prev = prev;
        } else {
            head->last = prev;
        }

        // free memory
        if (head->deleteData != NULL) {
            (*(head->deleteData)) (head->current->data);
        }
        free(head->current);

        head->current = next;

        head->nbElements--;

        return head->current;
    }

    return NULL;
}


void list_deleteAll(LIST_HEAD *head) {
    if (head != NULL) {
        (void) list_goToFirst(head);

        while (list_deleteCurrent(head) != NULL);
    }
}

// de maniere croissant
void list_sort(LIST_HEAD *head) {
    bool sorted = false;
    int i=0;
    LIST_ELEMENT *tmpLast = NULL;
    
    if (head!=NULL && head->sortData!=NULL) {

        tmpLast = head->last;

        for (i=0 ; i<head->nbElements && !sorted ; i++) {

            (void) list_goToFirst(head);
            sorted = true;

            while (head->current != tmpLast) {
                if ((*(head->sortData))(head->current->data,
                                        head->current->next->data) == 1) {
                    swap_data(head->current, head->current->next);
                    sorted = false;
                }
                list_goToNext(head);
            }

            tmpLast = tmpLast->prev;

        }

        #ifdef DEBUG
        printf("List sorted in %d/%d cycles\n", i, head->nbElements);
        #endif
    }
}

static void swap_data(LIST_ELEMENT *p_a, LIST_ELEMENT *p_b) {
    if (p_a!=NULL && p_b!=NULL) {
        void *tmpData = p_a->data;
        p_a->data = p_b->data;
        p_b->data = tmpData;
    }
}
