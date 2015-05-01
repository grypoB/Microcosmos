/* Nom: linked_list.h
 * Description: module qui gère des listes chaines
 * Date: 15.04.2015
 * version : 1.0
 * responsable du module : Alexandre Devienne
 * groupe : Alexandre Devienne, Pauline Maury Laribière
 */

#ifndef LINKED_LIST_H
#define LINKED_LIST_H

/* A quick guide to use this module which implements double linked list:
   - creation : see list_create
   - adding data : see list_add
   - delete data : see list_deleteCurrent and list_deleteAll
   - retrieving data : see list_getData and list_getDataFromId

   - retrieve info about the list :
        number of data entry : see list_getNbElements


   - moving through the data :
        - Core principle : like when reading a file, the LIST_HEAD remembers
                where its last position was
        - if a pointer on LIST_HEAD is required for a function call, you can
                expect the current position to change,
        - if it isn't a pointer, no data in the list will be
                added/removed/moved and the current position doesn't change
        - Go to the first data : see list_goToFirst
        - Go to next data : see list_goToNext
        - Go to a specific data (i.e : search) : see list_getDataFormId
        - Example :
            - cycling through all the data :
                if (list_goToFirst(&myList) != NULL) {
                    do {
                        myData = list_getData(myList, LIST_CURRENT);
                        ...
                        do whatever with your pointer myData
                        ...
                    } while (list_goToNext(&myList));
                }
            - To ease the work to call a function for all data elements see 
    			- list_fctToAllNext
                - list_fctToAllElements
                - list_fctToAll2Combinations

    - You can also sort datas (see list_sort to move through the data
            more efficiently)

 Warning : even if you could modify the field in the various structure,
    don't do it. Even to retrieve the fields of the LIST_HEAD structre
    the recommended way is to use the various getters.
    Otherwise don't expect any function to work properly.
    Also, you shouldn't have to use the LIST_ELEMENT structure
*/

#define LIST_CURRENT 0

typedef struct List_element LIST_ELEMENT;
typedef struct List_head    LIST_HEAD;

// Structure which should be exclusively handled by this module
struct List_element {                                               //?????????????? element particule/gen/bckh

    void *data; // pointer to data of the node/element

    // pointers to previous/next elements of the list
    LIST_ELEMENT *prev;
    LIST_ELEMENT *next;

};


// LIST_HEAD : the core structure which is used to store all the linked list
struct List_head {

    // Pointer to have quick access to elements of the list
    LIST_ELEMENT *first;
    LIST_ELEMENT *current;
    LIST_ELEMENT *last;

    // number of elements in the list
    int nbElements;

    // func called whenever an element is deleted
    // Do an action on delete of the data associated to the element
    // (ex : free(data))
    void (*deleteData) (void *data);
    // fct to sort (comparing two data)
    // must return -1 if a<b
    //              0 if a==b
    //              1 if a>b
    int (*sortData) (void *a, void *b);
    // return the id of data (see list_getDataFromId)
    int (*idOfData) (void *data);
};


// -----------
// Creation :
/**  returns an empty LIST_HEAD with functionality built in 
 *      (see LIST_HEAD structure for how these func should behave):
 *      - deleteData : func called whenever a data is removed
 *      - sortData   : func used to sort the data (see list_sort and the
 *                     LIST_HEAD structure)
 *     - idOfData    : func used to move quickly to a particular data
 *                     (see list_getDataFromId)
 * 
 * if you don't intend to use this functionallity, you can just send the
 * NULL value note the two following lines are equivalent 
 * (useful to declare initialized static variables) :
 *   LIST_HEAD head = {0};
 *   LIST_HEAD head = list_create(NULL, NULL, NULL);
 *
 * this LIST_HEAD will be needed to manipulate the linked list everytime
 */
LIST_HEAD list_create(void (*deleteData) (void *data),
                      int (*sortData) (void *a, void *b),
                      int (*idOfData) (void *data));


// -----------
// adds an element to the end of list, sets its data to data
// returns the new current position : the new element added
// exits if malloc fails
LIST_ELEMENT* list_add(LIST_HEAD *pHead, void *data);


// -----------
// funcs to delete elements
// every time an element is deleted, the func deleteData is called on its data
// (see structure HEAD_LIST)
// if deleteData == NULL, this step is ommited, but the elements of the
// list are still deleted

// delete current element
// see list_goToFirst, list_goToNext and list_getDataFromId
// if current == NULL, doesn't do anything
LIST_ELEMENT* list_deleteCurrent(LIST_HEAD *pHead); 
// delete all the elements of the list
// after that, LIST_HEAD is as it was when it was created with list_create
void          list_deleteAll    (LIST_HEAD *pHead); 


// ----------
// funcs to move or get data across the linked list

// move cursor to first element
// return a pointer to new current position (NULL, if empty list)
LIST_ELEMENT* list_goToFirst(LIST_HEAD *pHead);
// move cursor to last element
// return a pointer to new current position (NULL, if empty list)
LIST_ELEMENT* list_goToLast(LIST_HEAD *pHead);
// move cursor to the next element
// if current is the last one, current is set to NULL
// if current == NULL, go back to the first one
// return the new current position (NULL if reached end of list)
LIST_ELEMENT* list_goToNext (LIST_HEAD *pHead);

// retrieves the data from a specific position in the list (elementNB)
// elementNB designs which position in the list counts from 1 goes up to 
// list_getNbElements
// To retrieve value of current position send LIST_CURRENT (=0) value
// for best performance ask for current (constant vs linear time)
// does not modify current position
void* list_getData(LIST_HEAD head, int elementNB);

// return the data which id is the sent id parameter 
// (as asked to idOfData (see list_create))
// Begins its search form current pos, when end is reached, restarts at top
// if found, current now is this node
// if not found returns NULL, and current position kept
// if idOfData not set at list_create (i.e == NULL), doesn't do anything
void* list_getDataFromId(LIST_HEAD *pHead, int id); 


// ----------
// Returns the number of elements (ie. data) stored in the list
int   list_getNbElements(LIST_HEAD head);


// ----------
// sorts the list in an ascending way (see sortData in LIST_HEAD structure)
// sets current to first element
// if sortData not set at list_create (i.e == NULL), doesn't do anything
void list_sort(LIST_HEAD *pHead);


// ----------
// utility func to ease data manipulation : apply the same func to a specific
// set of data
// doesn't modify the current element

// call func wih parameter the data of all the elements of the list after 
// the current one.
// does not do it for the current one, hence if current==last,
// doesn't do anything, same for  current == NULL
void list_fctToAllNext    (LIST_HEAD head, void (*func) (void *data));

// call func with parameter the data of all the list's element
// hence func is called nbElements time
void list_fctToAllElements(LIST_HEAD head, void (*func) (void *data));

// func is called for every 2 combinations of element's data
// a and b should be interchangeable ( (a,b) is considered the same as (b,a) )
// hence func is called n(n+1)/2 times
void list_fctToAll2combinations(LIST_HEAD head, void (*func)(void*a, void*b));


#endif
