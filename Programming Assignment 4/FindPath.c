//////////////////////////////////////
//||  Geoff Huang (ghuang6)       ||//
//||  CMPS 101 Professor Tantalo  ||//
//||  Programming Assignment 4    ||//
//////////////////////////////////////

// ===================================================================================
// FindPath.c is the top level client module for programming assignment 4.
// It takes two command line arguments - an infile name and an outfile name.
// The first line of the infile must specify the number of vertices in the Graph.
// The second chunk of the infile must specify the edges of an undirected Graph.
// The third chunk of the infile must specify which vertices to find the shortest path
// to and from. The distance and the exact path are outputted to the outfile.
// ===================================================================================

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"Graph.h"

#define MAX_LEN 160

int main(int argc, char * argv[])
{
	FILE *in, *out;	// infile, outfile
	char line[MAX_LEN];	// string for fgets()
	int n, u = 1, v = 1;	// number of vertices, initial vertex, terminal vertex
	char *token;	// string array for strtok()

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

	// initialize Graph object
	Graph G = newGraph(n);
	
	// while the first "0 0" line is not reached
	while (strcmp(fgets(line, MAX_LEN, in), "0 0\n") != 0)
	{
		token = strtok(line, " \n");
		u = atoi(token);
		token = strtok(NULL, " \n");

		v = atoi(token);
		token = strtok(NULL, " \n");
		
		// add the edges into the adjacency list
		addEdge(G, u, v);
	}

	// output the adjacency list to the outfile
	printGraph(out, G);

	// while the second "0 0" line is not reached
	while (strcmp(fgets(line, MAX_LEN, in), "0 0\n") != 0)
	{
		token = strtok(line, " \n");
		u = atoi(token);
		token = strtok(NULL, " \n");

		v = atoi(token);
		token = strtok(NULL, " \n");

		// new List object (store shortest path)
		List L = newList();

		BFS(G, u);
		getPath(L, G, v);

		// print distance
		fprintf(out, "\nThe distance from %i to %i is ", u, v);
		if (getDist(G, v) == -1)
		{
			fprintf(out, "infinity\n");
		}
		else
		{
			fprintf(out, "%i\n", getDist(G, v));
		}

		// print shortest path
		if (front(L) == 0)
		{
			fprintf(out, "No %i-%i path exists\n", u, v);
		}
		else
		{
			fprintf(out, "A shortest %i-%i path is: ", u, v);
			printList(out, L);
			fprintf(out, "\n");
		}

		// free heap memory associated with the List I created
		freeList(&L);
	}

	// free heap memory associated with the Graph I created
	freeGraph(&G);

	// close files
	fclose(in);
	fclose(out);

	return(0);
}
