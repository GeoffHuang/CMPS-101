//////////////////////////////////////
//||  Geoff Huang (ghuang6)       ||//
//||  CMPS 101 Professor Tantalo  ||//
//||  Programming Assignment 5    ||//
//////////////////////////////////////

// ===================================================================================
// FindComponents.c is the top level client module for programming assignment 5.
// It takes two command line arguments - an infile name and an outfile name.
// The first line of the infile must specify the number of vertices in the Graph.
// The second chunk of the infile must specify the edges of a directed Graph (digraph).
// The strongly connected components of the digraph are outputted to the outfile,
// along with its adjacency list representation.
// ===================================================================================

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"Graph.h"

#define MAX_LEN 160

int main(int argc, char * argv[])
{
	FILE *in, *out; // infile, outfile
	char line[MAX_LEN]; // string for fgets()
	char *token; // string array for strtok()
	int n, u = 1, v = 1; // number of vertices in G, initial vertex, terminal vertex
	int numSCC = 0, component = 1; // #SCC in G, component #
	Graph G = NULL, GT = NULL; // G and G transpose
	List S = NULL; // inoutput/output stacks for DFS

	// check command line for correct number of arguments
	if (argc != 3)
	{
		printf("Usage: %s <input file> <output file>\n", argv[0]);
		exit(1);
	}

	// open files for reading and writing 
	in = fopen(argv[1], "r");
	out = fopen(argv[2], "w");
	if (in == NULL)
	{
		printf("Unable to open file %s for reading\n", argv[1]);
		exit(1);
	}
	if (out == NULL)
	{
		printf("Unable to open file %s for writing\n", argv[2]);
		exit(1);
	}

	// grab the first line (number of vertices)
	fgets(line, MAX_LEN, in);
	n = atoi(line);

	// Graphs G, GT (G transpose), and List S ("stack")
	G = newGraph(n);
	
	// while the first "0 0" line is not reached
	while (strcmp(fgets(line, MAX_LEN, in), "0 0\n") != 0)
	{
		token = strtok(line, " \n");
		u = atoi(token);
		token = strtok(NULL, " \n");

		v = atoi(token);
		token = strtok(NULL, " \n");
		
		// add the directed edges into the adjacency list of the digraph
		addArc(G, u, v);
	}

	// output the adjacency list to the outfile
	fprintf(out, "Adjacency list representation of G:\n");
	printGraph(out, G);

	///////////////////////
	//| Find SCC's of G |//
	///////////////////////

	// 1. Run DFS on G (ascending vertex labels)
	S = newList();
	for (int i = 1; i <= n; i++)
	{
		append(S, i);
	}
	DFS(G, S);

	// 2. Run DFS on G transpose (descending finish times from 1)
	// (the DFS function I wrote altered S already to do this)
	GT = transpose(G);
	DFS(GT, S);

	// (output the number of strongly connected components)
	// (aka # trees aka # root vertices)
	for (int i = 1; i <= n; i++)
	{
		if (getParent(GT, i) == NIL)
		{
			numSCC++;
		}
	}
	fprintf(out, "\nG contains %i strongly connected components:", numSCC);

	// 3. Output the vertices of each tree in the DFS forest
	// note: S is already "sorted" into SCC's, separated by root vertices
	while (!isEmpty(S))
	{
		fprintf(out, "\nComponent %i:", component);
		moveBack(S);
		while (getParent(GT, get(S)) != NIL)
		{
			movePrev(S);
		}
		while (index(S) >= 0)
		{
			fprintf(out, " %i", get(S));
			moveNext(S);
		}
		while (getParent(GT, back(S)) != NIL)
		{
			deleteBack(S);
		}
		deleteBack(S);

		component++;
	}

	// free heap memory associated with the Graphs/List I created
	freeGraph(&G);
	freeGraph(&GT);
	freeList(&S);

	// close files
	fclose(in);
	fclose(out);

	return(0);
}