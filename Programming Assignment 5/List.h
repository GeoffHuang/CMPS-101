//////////////////////////////////////
//||  Geoff Huang (ghuang6)       ||//
//||  CMPS 101 Professor Tantalo  ||//
//||  Programming Assignment 5    ||//
//////////////////////////////////////

// ============================================================================
// List.h is the header file for List.c
// ============================================================================

#ifndef _LIST_H_INCLUDE_
#define _LIST_H_INCLUDE_

// Exported type --------------------------------------------------------------
typedef struct ListObj* List;

// Constructors-Destructors ---------------------------------------------------
List newList(void);
void freeList(List* pL);

// Access Functions -----------------------------------------------------------
int isEmpty(List L);
int length(List L);
int index(List L);
int front(List L);
int back(List L);
int get(List L);
int equals(List A, List B);

// Manipulation Procedures ----------------------------------------------------
void clear(List L);
void moveFront(List L);
void moveBack(List L);
void movePrev(List L);
void moveNext(List L);
void prepend(List L, int data);
void append(List L, int data);
void insertBefore(List L, int data);
void insertAfter(List L, int data);
void sortedInsert(List L, int v);
void deleteFront(List L);
void deleteBack(List L);
void delete(List L);

// Other Functions ------------------------------------------------------------
void printList(FILE* out, List L);
List copyList(List L);

#endif
