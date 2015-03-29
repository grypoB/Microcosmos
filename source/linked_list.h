/* Nom: linked_list.h
 * Description: module qui gère les liste chaine
 * Date: 22.03.2015
 * version : 1.0
 * responsable du module : Alexandre Devienne
 * groupe : Alexandre Devienne, Pauline Maury Laribière
 */

#ifndef LINKED_LIST_H
#define LINKED_LIST_H

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

    // fct to compare
    // must return -1 if a<b
    //              0 if a==b
    //              1 if a>b
    int (*sortData) (void *a, void *b);

    // fct used to search

};




LIST_HEAD list_create(void (*deleteData) (void *data), int (*sortData) (void *a, void *b));

LIST_ELEMENT* list_goToFirst(LIST_HEAD *head);
LIST_ELEMENT* list_goToNext (LIST_HEAD *head);

void list_sort(LIST_HEAD *head);

void list_fctToAllNext(LIST_HEAD *head, void (*func) (void *data));
void list_fctToAllElements(LIST_HEAD *head, void (*func) (void *data));

LIST_ELEMENT* list_add(LIST_HEAD *head, void *data);

LIST_ELEMENT* list_deleteCurrent(LIST_HEAD *head); 
void list_deleteAll(LIST_HEAD *head); 

#endif
