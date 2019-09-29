//////////////////////////////////////
//||  Geoff Huang (ghuang6)       ||//
//||  CMPS 101 Professor Tantalo  ||//
//||  Programming Assignment 4    ||//
//////////////////////////////////////

// ============================================================================
// Graph.c is a Graph ADT that stores its information in the form of an
// adjacency List.
// ============================================================================

#include<stdio.h>
#include<stdlib.h>
#include "Graph.h"

// Structs ====================================================================

// private GraphObj type
typedef struct GraphObj
{
	int order;        // number of vertices
	int size;         // number of edges
	int source;       // vertex that was most recently used as the BFS source

	// note: index 0 is NOT used; vertex labels start at 1
	// all arrays have size n + 1 where n is the number of vertices
	List *adjList;    // adjacency List (note: index 0 is not used)
	char *color;      // color of the vertex (white, gray, black)
	int *parent;      // parent array
	int *distance;    // distance from the most recent source to each vertex
} GraphObj;

// Constructors-Destructors ===================================================

// returns reference to new empty Graph object
Graph newGraph(int n)
{
	Graph G;
	G = malloc(sizeof(GraphObj));

	// note: index 0 is NOT used; vertex labels start at 1
	// all arrays have size n + 1 where n is the number of vertices
	G->adjList = malloc((n+1)*sizeof(List));
	G->color = malloc((n+1)*sizeof(char));
	G->parent = malloc((n+1)*sizeof(int));
	G->distance = malloc((n+1)*sizeof(int));

	G->order = n;
	G->size = 0;
	G->source = NIL;

	for (int i = 1; i <= n; i++)
	{
		G->adjList[i] = newList();
	}

	return G;
}

// frees all heap memory associated with Graph *pG, and sets *pG to NULL
void freeGraph(Graph *pG)
{
	if (pG != NULL && *pG != NULL) 
	{
		// free adjacency list
		for (int i = 1; i <= getOrder(*pG); i++)
		{ 
			freeList(&(*pG)->adjList[i]);
		}
		free((*pG)->adjList);
		(*pG)->adjList = NULL;

		// free color, parent, distance arrays
		free((*pG)->color);
		(*pG)->color = NULL;
		free((*pG)->parent);
		(*pG)->parent = NULL;
		free((*pG)->distance);
		(*pG)->distance = NULL;

		// free pointer to Graph object
		free(*pG);
		*pG = NULL;
	}
}

// Access Functions ===========================================================

// returns the order (number of vertices)
int getOrder(Graph G)
{
	if (G == NULL)
	{
		printf("Graph Error: calling getOrder() on NULL Graph reference\n");
		exit(1);
	}
	return G->order;
}

// returns the size (number of edges)
int getSize(Graph G)
{
	if (G == NULL)
	{
		printf("Graph Error: calling getSize() on NULL Graph reference\n");
		exit(1);
	}
	return G->size;
}

// returns the source vertex most recently used in BFS
// returns NIL if BFS has not been called
int getSource(Graph G)
{
	if (G == NULL)
	{
		printf("Graph Error: calling getSource() on NULL Graph reference\n");
		exit(1);
	}
	return G->source;
}

// returns vertex u's parent in the BFS tree
// returns NIL if BFS has not yet been called
int getParent(Graph G, int u)
{
	if (G == NULL)
	{
		printf("Graph Error: calling getParent() on NULL Graph reference\n");
		exit(1);
	}
	// if BFS has not yet been called
	if (getSource(G) == NIL)
	{
		return NIL;
	}
	// note: instead of above, I could have initialized parent array to all NIL
	return G->parent[u];
}

// returns the distance from the most recent BFS source to vertex u
// returns INF if BFS has not yet been called
int getDist(Graph G, int u)
{
	if (G == NULL)
	{
		printf("Graph Error: calling getDist() on NULL Graph reference\n");
		exit(1);
	}
	// if BFS has not yet been called
	if (getSource(G) == NIL)
	{
		return INF;
	}
	// note: instead of above, I could have initialized distance array to all INF
	return G->distance[u];
}

// appends to List L the vertices of a shortest path in G from source to u
// if no path exists, append NIL
// pre: BFS() must have been called (source is not NIL)
void getPath(List L, Graph G, int u)
{
	if (getSource(G) == NIL)
	{
		printf("Graph Error: calling getPath() without having first called BFS()\n");
		exit(1);
	}

	if (u == G->source)
	{
		append(L, u);
	}
	else if (getParent(G, u) == NIL)
	{
		append(L, NIL);
	}
	else
	{
		getPath(L, G, getParent(G, u));
		append(L, u);
	}
}

// Manipulation Procedures ====================================================

// helper function that inserts an integer into a List so that the List is
// in ascending numerical order
// (is it more logical to put this as a helper function in List.c?)
void sortedInsert(List L, int v)
{
	if (L == NULL)
	{
		printf("Graph Error: calling sortedInsert() on NULL List reference\n");
		exit(1);
	}

	// check the contents of the List
	moveFront(L);
	while (index(L) >= 0)
	{
		// if v is less than the cursor element, insert before cursor
		if (v < get(L))
		{
			insertBefore(L, v);

			return;
		}
		moveNext(L);
	}

	// if v is greater than all the current values in the List, append
	// note: if List is empty, this case applies
	append(L, v);
}

// deletes all edges of G (null graph)
void makeNull(Graph G)
{
	if (G == NULL)
	{
		printf("Graph Error: calling makeNull() on NULL Graph reference\n");
		exit(1);
	}

	for (int i = 1; i <= getOrder(G); i++)
	{
		clear(G->adjList[i]);
	}
}

// inserts a new undirected edge joining u to v
// pre: 1 <= u,v <= getOrder(G)
void addEdge(Graph G, int u, int v)
{
	if (G == NULL)
	{
		printf("Graph Error: calling addEdge() on NULL Graph reference\n");
		exit(1);
	}
	if (u < 1 || u > getOrder(G))
	{
		printf("Graph Error: calling addEdge() with invalid vertex u()\n");
		exit(1);
	}
	if (v < 1 || v > getOrder(G))
	{
		printf("Graph Error: calling addEdge() with invalid vertex v()\n");
		exit(1);
	}

	sortedInsert(G->adjList[u], v);
	sortedInsert(G->adjList[v], u);
	G->size++;
	// note: there is no error check to check for duplicate edges in the infile
	// if the infile contains duplicate edges, the size of the graph will be off
}

// inserts a new directed edge from u to v
// pre: 1 <= u,v <= getOrder(G)
void addArc(Graph G, int u, int v)
{
	if (G == NULL)
	{
		printf("Graph Error: calling addArc() on NULL Graph reference\n");
		exit(1);
	}
	if (u < 1 || u > getOrder(G))
	{
		printf("Graph Error: calling addArc() with invalid vertex u()\n");
		exit(1);
	}
	if (v < 1 || v > getOrder(G))
	{
		printf("Graph Error: calling addArc() with invalid vertex v()\n");
		exit(1);
	}

	sortedInsert(G->adjList[u], v);
	G->size++;
	// note: there is no error check to check for duplicate edges in the infile
	// if the infile contains duplicate edges, the size of the graph will be off
}

// BREADTH FIRST SEARCH
// (finds single source shortest path to all reachable vertices)
void BFS(Graph G, int s)
{
	if (G == NULL)
	{
		printf("Graph Error: calling BFS() on NULL Graph reference\n");
		exit(1);
	}

	G-> source = s;

	for (int i = 1; i <= getOrder(G); i++)
	{
		G->color[i] = 'w';
		G->distance[i] = INF;
		G->parent[i] = NIL;
	}
	G->color[s] = 'g';
	G->distance[s] = 0;
	// note: the parent of the source is already NIL from for loop

	List Queue = newList();
	prepend(Queue, s);	// "enqueue"

	while (length(Queue) > 0)
	{
		int x = back(Queue);
		moveFront(G->adjList[x]);
		while (index(G->adjList[x]) >= 0)
		{
			int y = get(G->adjList[x]);
			if (G->color[y] == 'w')
			{
				G->color[y] = 'g';
				G->distance[y] = G->distance[x] + 1;
				G->parent[y] = x;
				prepend(Queue, y);
			}
			moveNext(G->adjList[x]);
		}
		deleteBack(Queue);
		G->color[x] = 'b';
	}

	freeList(&Queue);
}

// Other Functions ============================================================

// outputs the adjacency list to the outfile
void printGraph(FILE *out, Graph G)
{
	if (G == NULL)
	{
		printf("Graph Error: calling printGraph() on NULL Graph reference\n");
		exit(1);
	}

	for (int i = 1; i <= getOrder(G); i++)
	{
		fprintf(out, "%d: ", i);
		printList(out, G->adjList[i]);
		fprintf(out, "\n");
	}
}