/* Nom: linked_list.h
 * Description: module qui gère des listes chaines
 * Date: 22.03.2015
 * version : 1.0
 * responsable du module : Alexandre Devienne
 * groupe : Alexandre Devienne, Pauline Maury Laribière
 */

#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#define LIST_CURRENT 0

typedef struct List_element LIST_ELEMENT;
typedef struct List_head    LIST_HEAD;

struct List_element {

    void *data;

    LIST_ELEMENT *prev;
    LIST_ELEMENT *next;

};


struct List_head {

    LIST_ELEMENT *first;
    LIST_ELEMENT *current;
    LIST_ELEMENT *last;

    int nbElements;

    //fct to delete data
    void (*deleteData) (void *data);

    // fct to sort (comparing two data
    // must return -1 if a<b
    //              0 if a==b
    //              1 if a>b
    int (*sortData) (void *a, void *b);

    // fct to find a particular data entry form it's id
    int (*idOfData) (void *data);
};


LIST_HEAD list_create(void (*deleteData) (void *data),
                      int (*sortData) (void *a, void *b),
                      int (*idOfData) (void *data));

LIST_ELEMENT* list_goToFirst(LIST_HEAD *pHead);
LIST_ELEMENT* list_goToNext (LIST_HEAD *pHead);
LIST_ELEMENT* list_goToLast (LIST_HEAD *pHead);

void list_sort(LIST_HEAD *pHead);

void list_fctToAllNext    (LIST_HEAD head, void (*func) (void *data));
void list_fctToAllElements(LIST_HEAD head, void (*func) (void *data));
void list_fctToAll2combinations(LIST_HEAD head, void (*func) (void *a, void *b));

LIST_ELEMENT* list_add(LIST_HEAD *pHead, void *data);

LIST_ELEMENT* list_deleteCurrent(LIST_HEAD *pHead); 
void          list_deleteAll    (LIST_HEAD *pHead); 

// some getters
int   list_getNbElements(LIST_HEAD head);
// counts from 1 (LIST_CURRENT = 0)
// for best performance ask for current (constant vs linear time)
// do not modify current pos
void* list_getData(LIST_HEAD head, int elementNB);
void* list_getDataFromId(LIST_HEAD *pHead, int id); 

#endif
