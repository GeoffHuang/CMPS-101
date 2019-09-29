//////////////////////////////////////
//||  Geoff Huang (ghuang6)       ||//
//||  CMPS 101 Professor Tantalo  ||//
//||  Programming Assignment 2    ||//
//////////////////////////////////////


#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"List.h"

#define MAX_LEN 160

int main(int argc, char * argv[])
{
	int lines = 0, s_index = 0;
	FILE *in, *out;
	char line[MAX_LEN];

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

	// count number of lines in input file
	while (fgets(line, MAX_LEN, in) != NULL)  
	{
		lines++;
	}

	// create a String array with that many strings
	//char stringArray[lines][MAX_LEN]; 
	// Note: the above (comment) should work on a c99 compliant compiler, but it doesn't work on my compiler
	// Remember to uncomment before submitting
	char stringArray[999][MAX_LEN];

	// store each line of the infile into the string array
	rewind(in); // rewind to the beginning of the file
	while (fgets(line, MAX_LEN, in) != NULL)
	{
		strcpy(stringArray[s_index], line);
		s_index++;
	}
	
	List L = newList();
	append(L, 0); // add the first index to the List

	// basically using the insertion sort algorithm to create a sorted
	// List using the indices of the above String array
	for (int i = 1; i < s_index; i++)
	{
		// start from the beginning of the List and shift the cursor until
		// the first "string" in the List that is lexicographically greater is found
		moveFront(L);
		while (index(L) >= 0 && strcmp(stringArray[i], stringArray[get(L)]) > 0)
		{
			moveNext(L);
		}
		// if no "strings" in the current List are lexicographically greater
		if (index(L) == -1)
		{
			// add to the back of the List
			append(L, i);
		}
		else
		{
			// otherwise, insert before the first "string" in the List that is lexicographically greater
			insertBefore(L, i);
		}
	}

	// now that the indices are sorted, use them to write the sorted strings to the outfile
	moveFront(L);
	while (index(L) >= 0)
	{
		fprintf(out, "%s ", stringArray[get(L)]);
		moveNext(L);
	}
		
	fprintf(out, "\n");

	// close files
	fclose(in);
	fclose(out);

	// free heap memory associated with the List I created
	freeList(&L);

	return(0);
}
