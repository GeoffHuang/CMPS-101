//////////////////////////////////////
//||  Geoff Huang (ghuang6)       ||//
//||  CMPS 101 Professor Tantalo  ||//
//||  Programming Assignment 4    ||//
//////////////////////////////////////

// ============================================================================
// List.c is a doubly linked List ADT that stores integers in the nodes.
// ============================================================================

#include<stdio.h>
#include<stdlib.h>
#include "List.h"

// Structs ====================================================================

// private NodeObj type
typedef struct NodeObj
{
	int data;
	struct NodeObj *next;
	struct NodeObj *prev;
} NodeObj;

// private Node type
typedef NodeObj *Node;

// private ListObj type
typedef struct ListObj
{
	Node front;
	Node back;
	Node cursor;
	int length;
	int index;
} ListObj;


// Constructors-Destructors ===================================================

// Returns reference to new Node object. Initializes next and data fields
// Private
Node newNode(int data)
{
	Node N = malloc(sizeof(NodeObj));
	N->data = data;
	N->next = N->prev = NULL;
	return (N);
}

// Frees heap memory pointed to by *pN, sets *pN to NULL
// Private
void freeNode(Node *pN)
{
	if (pN != NULL && *pN != NULL)
	{
		free(*pN);
		*pN = NULL;
	}
}

// Returns reference to new empty List object
List newList(void)
{
	List L;
	L = malloc(sizeof(ListObj));
	L->front = L->back = NULL;
	L->cursor = NULL;
	L->length = 0;
	L->index = -1;
	return (L);
}

// Frees all heap memory associated with List *pL, and sets *pL to NULL
void freeList(List *pL)
{
	if (pL != NULL && *pL != NULL) 
	{ 
		while (!isEmpty(*pL)) 
		{ 
			deleteFront(*pL); // or deleteBack(*pL)
			// NOTE: deleteFront() frees Nodes
		}
		free(*pL);
		*pL = NULL;
	}
}


// Access Functions ===========================================================

// Returns true (1) if L is empty, otherwise returns false (0)
int isEmpty(List L)
{
	if (L == NULL)
	{
		printf("List Error: calling isEmpty() on NULL List reference\n");
		exit(1);
	}
	return (L->length == 0);
}

// Returns the number of elements in the List
int length(List L)
{
	if (L == NULL)
	{
		printf("List Error: calling length() on NULL List reference\n");
		exit(1);
	}
	return (L->length);
}

// If cursor is defined, returns the index of the cursor element
// Otherwise returns -1
int index(List L)
{
	if (L == NULL)
	{
		printf("List Error: calling index() on NULL List reference\n");
		exit(1);
	}
	return L->index; // note: L->index = -1 when cursor is undefined
}

// Returns the value at the front of L
int front(List L)
{
	if (L == NULL)
	{
		printf("List Error: calling front() on NULL List reference\n");
		exit(1);
	}
	if (isEmpty(L))
	{
		printf("List Error: calling front() on an empty List\n");
		exit(1);
	}
	return (L->front->data);
}

// Returns the value at the back of L
int back(List L)
{
	if (L == NULL)
	{
		printf("List Error: calling back() on NULL List reference\n");
		exit(1);
	}
	if (isEmpty(L))
	{
		printf("List Error: calling back() on an empty List\n");
		exit(1);
	}
	return (L->back->data);
}

// Returns cursor element of L
int get(List L)
{
	if (L == NULL)
	{
		printf("List Error: calling get() on NULL List reference\n");
		exit(1);
	}
	if (isEmpty(L))
	{
		printf("List Error: calling get() on empty List");
		exit(1);
	}
	if (index(L) == -1)
	{
		printf("List Error: calling get() when cursor is not pointing at anything");
		exit(1);      
	}
	return (L->cursor->data);
}

// returns true (1) if A is identical to B, false (0) otherwise
int equals(List A, List B)
{
	int eq = 0;
	Node N = NULL;
	Node M = NULL;

	if (A == NULL || B == NULL)
	{
		printf("List Error: calling equals() on NULL List reference\n");
		exit(1);
	}

	eq = (A->length == B->length);
	N = A->front;
	M = B->front;
	while (eq && N != NULL)
	{
		eq = (N->data == M->data);
		N = N->next;
		M = M->next;
	}
	return eq;
}


// Manipulation Procedures ====================================================

// Resets this List to its original empty state
void clear(List L)
{
	if (L == NULL)
	{
		printf("List Error: calling clear() on NULL List reference\n");
		exit(1);
	}

	while (length(L) > 0)
	{
		deleteFront(L); // or deleteBack(L)
	}
}

// If List is non-empty, places the cursor under the front element
// Otherwise, does nothing
void moveFront(List L)
{
	if (L == NULL)
	{
		printf("List Error: calling moveFront() on NULL List reference\n");
		exit(1);
	}

	if (length(L) > 0)
	{
		L->cursor = L->front;
		L->index = 0;
	}
}

// If List is non-empty, places the cursor under the back element
// Otherwise, does nothing
void moveBack(List L)
{
	if (L == NULL)
	{
		printf("List Error: calling moveBack() on NULL List reference\n");
		exit(1);
	}

	if (length(L) > 0)
	{
		L->cursor = L->back;
		L->index = length(L) - 1;
	}
}

// If cursor is defined and not at front, moves cursor one step towards front
// If cursor is defined and at front, cursor becomes undefined
// If cursor is undefined, does nothing
void movePrev(List L)
{
	if (L == NULL)
	{
		printf("List Error: calling movePrev() on NULL List reference\n");
		exit(1);
	}

	if (L->cursor != NULL)
	{
		L->cursor = L->cursor->prev;
		L->index--;
	}
}

// If cursor is defined and not at back, moves cursor one step towards back
// If cursor is defined and at back, cursor becomes undefined
// If cursor is undefined, does nothing
void moveNext(List L)
{
	if (L == NULL)
	{
		printf("List Error: calling moveNext() on NULL List reference\n");
		exit(1);
	}

	if (L->cursor != NULL)
	{
		L->cursor = L->cursor->next;
		L->index++;
		if (index(L) > length(L) - 1)
		{
			L->index = -1;
		}
	}
}

// Insert new element into this List
// If List is non-empty, insertion takes place before front element
void prepend(List L, int data)
{
	Node N = newNode(data);

	if (L == NULL)
	{
		printf("List Error: calling prepend() on NULL List reference\n");
		exit(1);
	}
	if (isEmpty(L)) 
	{ 
		L->front = L->back = N;
	}
	else
	{ 
		L->front->prev = N; 
		N->next = L->front;
		L->front = N;
	}
	if (index(L) >= 0)
	{
		L->index++;
	}

	L->length++;
}

// Insert new element into this List
// If List is non-empty, insertion takes place after back element
void append(List L, int data)
{
	Node N = newNode(data);

	if (L == NULL)
	{
		printf("List Error: calling prepend() on NULL List reference\n");
		exit(1);
	}
	if (isEmpty(L)) 
	{ 
		L->front = L->back = N;
	}
	else
	{ 
		L->back->next = N; 
		N->prev = L->back;
		L->back = N; 
	}

	L->length++;
}

// Insert new element before cursor
// Pre: length() > 0, index >= 0
void insertBefore(List L, int data)
{
	if (isEmpty(L))
	{
		printf("List Error: insertBefore() called on empty List\n");
		exit(1);
	}
	if (index(L) < 0)
	{
		printf("List Error: insertBefore() called when cursor is not pointing at anything\n");
		exit(1);
	}

	Node N = newNode(data);  
	N->prev = L->cursor->prev;
	N->next = L->cursor;
	L->cursor->prev = N;
	if (L->cursor == L->front)
	{
		L->front = N;
	}
	else
	{
		N->prev->next = N;
	}

	L->index++;
	L->length++;
}

// Insert new element after cursor
// Pre: length() > 0, index >= 0
void insertAfter(List L, int data)
{
	if (isEmpty(L))
	{
		printf("List Error: insertAfter() called on empty List\n");
		exit(1);
	}
	if (index(L) < 0)
	{
		printf("List Error: insertAfter() called when cursor is not pointing at anything\n");
		exit(1);
	}

	Node N = newNode(data);        
	N->next = L->cursor->next;
	N->prev = L->cursor;
	L->cursor->next = N;
	if (L->cursor == L->back)
	{
		L->back = N;
	}
	else
	{
		N->next->prev = N;
	}

	L->length++;
}

// Deletes the front element
void deleteFront(List L)
{
	if (L == NULL)
	{
		printf("List Error: calling deleteFront() on NULL List reference\n");
		exit(1);
	}
	if (isEmpty(L))
	{
		printf("List Error: calling deleteFront() on an empty List\n");
		exit(1);
	}

	Node N = L->front;

	// change cursor if pointing at front
	// (do "nothing" if it's not pointing at anything)
	if (index(L) <= 0)
	{
		L->cursor = NULL;
		L->index = -1;
	}
	else
	{
		L->index--;
	}

	// change front element
	if (length(L) == 1)
	{
		L->front = L->back = NULL;
	}
	else
	{
		L->front = L->front->next;
		L->front->prev = NULL;
	}

	L->length--; // decrement length
	freeNode(&N); // frees heap memory pointed at by N
}

// Deletes the back element
void deleteBack(List L)
{
	if (L == NULL)
	{
		printf("List Error: calling deleteBack() on NULL List reference\n");
		exit(1);
	}
	if (isEmpty(L))
	{
		printf("List Error: calling deleteBack() on an empty List\n");
		exit(1);
	}

	Node N = L->back;

	// change cursor if pointing at back element
	if (index(L) == length(L) - 1)
	{
		L->cursor = NULL;
		L->index = -1;
	}

	// change back element
	if (length(L) == 1)
	{
		L->front = L->back = NULL;
	}     
	else
	{
		L->back = L->back->prev;
		L->back->next = NULL;
	}

	L->length--; // decrement length
	freeNode(&N); // frees heap memory pointed at by N
}

// Deletes cursor element, making cursor undefined
void delete(List L)
{
	if (L == NULL)
	{
		printf("List Error: calling delete() on NULL List reference\n");
		exit(1);
	}
	if (isEmpty(L))
	{
		printf("List Error: calling delete() on an empty List\n");
		exit(1);
	}
	if (index(L) < 0)
	{
		printf("List Error: delete() called when cursor is not pointing at anything\n");
		exit(1);
	}

	if (L->cursor == L->front)
	{
		deleteFront(L);
	}
	else if (L->cursor == L->back)
	{
		deleteBack(L);
	}
	else
	{
		Node N = L->cursor;
		L->cursor->prev->next = L->cursor->next;
		L->cursor->next->prev = L->cursor->prev;
		L->length--;
		freeNode(&N); // frees heap memory pointed at by N
	}

	L->cursor = NULL;
	L->index = -1;
}


// Other Functions ============================================================

// Prints data elements in Q on a single line to stdout.
void printList(FILE *out, List L)
{
	if (L == NULL)
	{
		printf("List Error: calling printList() on NULL List reference\n");
		exit(1);
	}

	Node N = NULL;
	for (N = L->front; N != NULL; N = N->next)
	{
		fprintf(out, "%d ", N->data);
	}
	// freeNode(&N);
}

// Returns a new List identical to this L
List copyList(List L)
{
	List L2 = newList();

	Node N = L->front;
	while (N != NULL)
	{
		append(L2, N->data);
		N = N->next;
	}

	return L2;
}