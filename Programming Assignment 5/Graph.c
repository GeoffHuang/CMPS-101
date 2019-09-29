//////////////////////////////////////
//||  Geoff Huang (ghuang6)       ||//
//||  CMPS 101 Professor Tantalo  ||//
//||  Programming Assignment 5    ||//
//////////////////////////////////////

// ============================================================================
// Graph.c is a Graph ADT that stores its information in the form of an
// adjacency List, and can run DFS given an input stack (which determines the
// order in which vertices are processed).
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

	// note: index 0 is NOT used; vertex labels start at 1
	// all arrays have size n + 1 where n is the number of vertices
	List *adjList;    // adjacency List (note: index 0 is not used)
	char *color;      // color of the vertex (white, gray, black)
	int *parent;      // parent of the vertex
	int *discover;     // discover time of the vertex in DFS
	int *finish;       // finish time of the vertex in DFS
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
	G->discover = malloc((n+1)*sizeof(int));
	G->finish = malloc((n+1)*sizeof(int));

	// initialize arrays (and create new List objects)
	for (int i = 1; i <= n; i++)
	{
		G->adjList[i] = newList();
		G->color[i] = 'w';
		G->parent[i] = NIL;
		G->discover[i] = UNDEF;
		G->finish[i] = UNDEF;
	}

	G->order = n;
	G->size = 0;

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

		// free color, parent, discover, finish arrays
		free((*pG)->color);
		(*pG)->color = NULL;
		free((*pG)->parent);
		(*pG)->parent = NULL;
		free((*pG)->discover);
		(*pG)->discover = NULL;
		free((*pG)->finish);
		(*pG)->finish = NULL;

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

// returns the color of vertex u
// pre: 1 <= u,v <= getOrder(G)
char getColor(Graph G, int u)
{
	if (G == NULL)
	{
		printf("Graph Error: calling getColor() on NULL Graph reference\n");
		exit(1);
	}
	if (u < 1 || u > getOrder(G))
	{
		printf("Graph Error: calling getColor() with invalid vertex u()\n");
		exit(1);
	}
	return G->color[u];
}

// returns the parent of vertex u in the DFS forest
// pre: 1 <= u,v <= getOrder(G)
// note: returns NIL if vertex u is the root of a tree, or DFS hasn't been called yet
int getParent(Graph G, int u)
{
	if (G == NULL)
	{
		printf("Graph Error: calling getParent() on NULL Graph reference\n");
		exit(1);
	}
	if (u < 1 || u > getOrder(G))
	{
		printf("Graph Error: calling getParent() with invalid vertex u()\n");
		exit(1);
	}
	return G->parent[u];
}

// returns the discover time of vertex u (UNDEF if DFS has not been called)
// pre: 1 <= u,v <= getOrder(G)
int getDiscover(Graph G, int u)
{
	if (G == NULL)
	{
		printf("Graph Error: calling getDiscover() on NULL Graph reference\n");
		exit(1);
	}
	if (u < 1 || u > getOrder(G))
	{
		printf("Graph Error: calling getDiscover() with invalid vertex u()\n");
		exit(1);
	}
	return G->discover[u];
}

// returns the finish time of vertex u (UNDEF if DFS has not been called)
// pre: 1 <= u,v <= getOrder(G)
int getFinish(Graph G, int u)
{
	if (G == NULL)
	{
		printf("Graph Error: calling getFinish() on NULL Graph reference\n");
		exit(1);
	}
	if (u < 1 || u > getOrder(G))
	{
		printf("Graph Error: calling getFinish() with invalid vertex u()\n");
		exit(1);
	}
	return G->finish[u];
}

// Manipulation Procedures ====================================================

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

// private helper function
// the recursive Visit() function that is used in DFS()
int Visit(Graph G, List S, int x, int time)
{
	G->color[x] = 'g';
	G->discover[x] = ++time;

	moveFront(G->adjList[x]);
	while (index(G->adjList[x]) >= 0)
	{
		int y = get(G->adjList[x]);
		if (G->color[y] == 'w')
		{
			G->parent[y] = x;
			time = Visit(G, S, y, time);
		}
		moveNext(G->adjList[x]);
	}
	G->color[x] = 'b';
	G->finish[x] = ++time;
	// push vertex x onto stack as it finishes
	insertAfter(S, x);

	return time;
}

// DEPTH FIRST SEARCH
// implementation details:
// List S is actually two stacks: input stack and output stack
// - this DFS function iterates through the vertices in input stack order
//   (vertices in the input stack are popped off the "front" of List S)
// - as vertices finish, they are pushed onto the output stack
//   (insertAfter the end of the input stack)
// I chose to make time a local variable in DFS(), and pass it to Visit()
void DFS(Graph G, List S)
{
	if (G == NULL)
	{
		printf("Graph Error: calling DFS() on NULL Graph reference\n");
		exit(1);
	}
	if (S == NULL || length(S) != getOrder(G))
	{
		printf("List Error: calling DFS() with invalid List S\n");
		exit(1);
	}

	for (int i = 1; i <= getOrder(G); i++)
	{
		G->color[i] = 'w';
		G->parent[i] = NIL;
	}

	// move List cursor to beginning of output stack
	// elements will be pushed onto this stack in DFS as they finish
	moveBack(S);

	int time = 0;
	// process vertices in input stack order
	for (int i = 1; i <= getOrder(G); i++)
	{
		int x = front(S);
		if (G->color[x] == 'w')
		{
			time = Visit(G, S, x, time);
		}
		deleteFront(S); // pop off input stack after processing
	}
}

// Other Functions ============================================================

// returns a reference to a new graph object representing the transpose of G
// (no change in undirected graphs)
// (edges reverse direction in digraphs)
// note: I don't copy G's color, parent, discover, or finish arrays
Graph transpose(Graph G)
{
	Graph GT = newGraph(getOrder(G));

	GT->order = getOrder(G); // 2019 check - redundant code?
	GT->size = getSize(G);

	// if vertex v is in vertex u's adjList
	// vertex u is now in vertex v's adjList
	for (int u = 1; u <= getOrder(G); u++)
	{
		moveFront(G->adjList[u]);
		while (index(G->adjList[u]) >= 0)
		{
			int v = get(G->adjList[u]);
			sortedInsert(GT->adjList[v], u);
			moveNext(G->adjList[u]);
		}
	}

	return GT;
}

// returns a reference to a new graph object which is a copy of G
// note: I copy G's color, parent, discover, and finish arrays
Graph copyGraph(Graph G)
{
	Graph G_Copy = newGraph(getOrder(G));

	G_Copy->order = getOrder(G); // 2019 check - redundant code?
	G_Copy->size = getSize(G);

	// initialize G_Copy's array contents with G's array contents
	for (int i = 1; i <= getOrder(G); i++)
	{
		G_Copy->adjList[i] = copyList(G->adjList[i]);
		G_Copy->color[i] = getColor(G, i);
		G_Copy->parent[i] = getParent(G, i);
		G_Copy->discover[i] = getDiscover(G, i);
		G_Copy->finish[i] = getFinish(G, i);
	}

	return G_Copy;
}

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