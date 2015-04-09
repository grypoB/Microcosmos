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

// for complete details on how these funcs behaves, see linked_list.h


static void swap_data(LIST_ELEMENT *p_a, LIST_ELEMENT *p_b);


LIST_HEAD list_create(void (*deleteData) (void *data),
                      int (*sortData) (void *a, void *b),
                      int (*idOfData) (void *data)) {

    LIST_HEAD head = {0};

    head.first   = NULL;
    head.current = NULL;
    head.last    = NULL;

    head.nbElements = 0;

    head.deleteData = deleteData;
    head.sortData = sortData;
    head.idOfData = idOfData;
    
    return head;
}


LIST_ELEMENT* list_goToFirst(LIST_HEAD *pHead) {
    LIST_ELEMENT *p = NULL;

    if (pHead != NULL) {
        pHead->current = pHead->first;
        p = pHead->current;
    }

    return p;
}

// if current == NULL, go back to first
LIST_ELEMENT* list_goToNext (LIST_HEAD *pHead) {
    LIST_ELEMENT *p = NULL;

    if (pHead != NULL) {
        if (pHead->current != NULL) {
            pHead->current = pHead->current->next;

        } else {
            pHead->current = pHead->first;
        }

        p = pHead->current;
    }

    return p;
}

LIST_ELEMENT* list_goToLast(LIST_HEAD *pHead) {
    LIST_ELEMENT *p = NULL;

    if (pHead != NULL) {
        pHead->current = pHead->last;
        p = pHead->current;
    }

    return p;
}


// doesn't change the position in the list (cause it's not a pointer)
// doesn't apply the func to current
void list_fctToAllNext(LIST_HEAD head, void (*func) (void *data)) {
    if (head.current!=NULL && func!=NULL) {

        while (list_goToNext(&head) != NULL) {
            (*func)(head.current->data);
        }
    }
}

// doesn't change the position in the list (because it's not a pointer)
void list_fctToAllElements(LIST_HEAD head, void (*func) (void *data)) {

        if (list_goToFirst(&head) != NULL) {
            do {
                (*func)(head.current->data);
            } while (list_goToNext(&head) != NULL);
        }
}

void list_fctToAll2combinations(LIST_HEAD head, void (*func) (void *a, void *b)) {
    LIST_ELEMENT *a = list_goToFirst(&head);
    LIST_ELEMENT *b = NULL;

    while (a != NULL) {
        b = a->next;

        while (b != NULL){
            (*func) (a->data, b->data);
            b = b->next;
        }

        a = a->next;
    }
}


// adds it at the end of the list
// return the pointer to the new node
LIST_ELEMENT* list_add(LIST_HEAD *pHead, void *data) {
    LIST_ELEMENT *p = NULL;

    if (pHead != NULL) {
        p = malloc(sizeof(LIST_ELEMENT));

        if (p == NULL) {
            printf("Memory allocation failure in %s\n", __func__);
            exit(EXIT_FAILURE);
        }

        if (pHead->last == NULL) { // list empty (then first also == NULL)
            pHead->first = p;
            p->prev = NULL;
        } else {
            pHead->last->next = p;
            p->prev = pHead->last;
        }

        p->data = data;
        p->next = NULL;
        pHead->last = p;

        pHead->nbElements++;
    }

    return p;
}


// return new current node
// i.e : the one after the deleted one
LIST_ELEMENT* list_deleteCurrent(LIST_HEAD *pHead) {
    LIST_ELEMENT *prev = NULL;
    LIST_ELEMENT *next = NULL;

    if (pHead!=NULL && pHead->current!=NULL) {
        prev = pHead->current->prev;
        next = pHead->current->next;

        if (prev != NULL) {
            prev->next = next;
        } else {
            pHead->first = next;
        }

        if (next != NULL) {
            next->prev = prev;
        } else {
            pHead->last = prev;
        }

        // free memory
        if (pHead->deleteData != NULL) {
            (*(pHead->deleteData)) (pHead->current->data);
        }
        free(pHead->current);

        pHead->current = next;

        pHead->nbElements--;

        return pHead->current;
    }

    return NULL;
}


// after : first,current,last = NULL
void list_deleteAll(LIST_HEAD *pHead) {
    if (pHead != NULL) {
        (void) list_goToFirst(pHead);

        // cycles through all modes to delete them one by one
        while (list_deleteCurrent(pHead) != NULL);
    }
}


int list_getNbElements(LIST_HEAD head) {
    return head.nbElements;
}


void* list_getData(LIST_HEAD head, int elementNB) {
    void *returnPointer = NULL;
    LIST_ELEMENT *el = NULL;
    int i = 0;

    if (elementNB==LIST_CURRENT && head.current!=NULL) {
        returnPointer = head.current->data;
    } else if (elementNB<=head.nbElements && elementNB>0) {

        el = head.first;

        for (i=1 ; i<elementNB ; i++) { // counts form 1
            if (el == NULL) {
                printf("FATAL ERROR in linked-list (%s)\n", __func__);
                exit(EXIT_FAILURE);
            }

            el = el->next;
        }
        returnPointer = el->data;
    }

    return returnPointer;
}


// change current of list
void* list_getDataFromId(LIST_HEAD *pHead, int id) {
    void *returnedData = NULL;
    LIST_ELEMENT *startingEl = NULL;
    bool found = false;

    if (pHead!=NULL && pHead->idOfData!=NULL) {
        startingEl = pHead->current;
        
        do {
            (void) list_goToNext(pHead);
            if ((*(pHead->idOfData))(pHead->current->data) == id) {
                found = true;
                returnedData = pHead->current->data;
            }
        } while (!found && startingEl!=pHead->current);
    }

    return returnedData;
}


// de maniere croissant
// the current then points to the first node
void list_sort(LIST_HEAD *pHead) {
    bool sorted = false;
    int i=0;
    LIST_ELEMENT *tmpLast = NULL;
    
    if (pHead!=NULL && pHead->sortData!=NULL) {

        tmpLast = pHead->last;

        for (i=0 ; i<pHead->nbElements && !sorted ; i++) {

            (void) list_goToFirst(pHead);
            sorted = true;

            while (pHead->current != tmpLast) {
                if ((*(pHead->sortData))(pHead->current->data,
                                        pHead->current->next->data) == 1) {
                    swap_data(pHead->current, pHead->current->next);
                    sorted = false;
                }
                (void) list_goToNext(pHead);
            }

            tmpLast = tmpLast->prev;

        }

        // to ensure the current node is the first one
        (void) list_goToFirst(pHead);

        #ifdef DEBUG
        printf("List sorted in %d/%d cycles\n", i, pHead->nbElements);
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
