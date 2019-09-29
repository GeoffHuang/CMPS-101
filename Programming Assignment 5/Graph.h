//////////////////////////////////////
//||  Geoff Huang (ghuang6)       ||//
//||  CMPS 101 Professor Tantalo  ||//
//||  Programming Assignment 5    ||//
//////////////////////////////////////

// ============================================================================
// Graph.h is the header file for Graph.c
// Note: It includes List.h
// ============================================================================

#include "List.h"
#define UNDEF -1 // undefined - applies to discover/finish times
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
char getColor(Graph G, int u);
int getParent(Graph G, int u);
int getDiscover(Graph G, int u);
int getFinish(Graph G, int u);

// Manipulation Procedures ====================================================
void addEdge(Graph G, int u, int v);
void addArc(Graph G, int u, int v);
void DFS(Graph G, List S);

// Other Functions ============================================================
Graph transpose(Graph G);
Graph copyGraph(Graph G);
void printGraph(FILE *out, Graph G);

#endif