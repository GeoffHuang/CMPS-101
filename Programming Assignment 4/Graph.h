//////////////////////////////////////
//||  Geoff Huang (ghuang6)       ||//
//||  CMPS 101 Professor Tantalo  ||//
//||  Programming Assignment 4    ||//
//////////////////////////////////////

// ============================================================================
// Graph.h is the header file for Graph.c
// ============================================================================

#include "List.h"
#define INF -1 // infinite - applies to distance
#define NIL 0  // "no vertex"

#ifndef _GRAPH_H_INCLUDE_
#define _GRAPH_H_INCLUDE_

// Exported type --------------------------------------------------------------
typedef struct GraphObj* Graph;

// Constructors-Destructors ===================================================
Graph newGraph(int n);
void freeGraph(Graph *pG);

// Access Functions ===========================================================
int getOrder(Graph G);
int getSize(Graph G);
int getSource(Graph G);
int getParent(Graph G, int u);
int getDist(Graph G, int u);
void getPath(List L, Graph G, int u);

// Manipulation Procedures ====================================================
void sortedInsert(List L, int v);
void makeNull(Graph G);
void addEdge(Graph G, int u, int v);
void addArc(Graph G, int u, int v);
void BFS(Graph G, int s);

// Other Functions ============================================================
void printGraph(FILE *out, Graph G);

#endif

