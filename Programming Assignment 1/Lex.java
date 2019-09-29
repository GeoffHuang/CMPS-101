//------------------------------------------------------------------------------
//  Geoff Huang (ghuang6@ucsc.edu)
//  CMPS 101 Professor Tantalo
//  Programming Assignment 1
//
//  Lex.java
//  This program uses the List.java class I wrote to indirectly sort a String 
//  array by insertion sorting it's indices into a List. As mentioned in
//  lecture, something like this could be useful if instead of an array of
//  Strings, we had an array where each element took up a significant amount
//  of memory and applying a sorting algorithm directly would be inefficient.
//------------------------------------------------------------------------------

import java.io.*;
import java.util.Scanner;

class Lex
{
    public static void main(String[] args) throws IOException
    {
        int lineNumber = 0;
        Scanner in1 = new Scanner(new File(args[0]));
        Scanner in2 = new Scanner(new File(args[0]));
        PrintWriter out = new PrintWriter(new FileWriter(args[1]));

        // Check that there are two command line arguments
        // Quit with a usage message to stderr if not
        if (args.length != 2)
        {
            System.err.println("Please enter only two arguments.");
            System.err.println("Usage: FileIO infile outfile");
            System.exit(1);
        }
      
        // Count the number of lines in the infile
        while (in1.hasNextLine())
        {
            lineNumber++;
            in1.nextLine();
        }

        // Create a String array with each element storing a line of the infile
        String[] stringArray = new String[lineNumber];
        int s_index = 0; // not to be confused with the index of the List cursor
        while (in2.hasNextLine())
        {
            stringArray[s_index] = in2.nextLine();
            s_index++;
        }

        List L = new List();
        L.append(0); // add the first index to the List

        // Basically using the insertion sort algorithm to create a sorted
        // List using the indices of the above String array
        for (int i = 1; i < s_index; i++)
        {
            L.moveFront();
            while (L.index() >= 0 && stringArray[i].compareTo(stringArray[L.get()]) > 0)
            {
                L.moveNext();
            }
            if (L.index() == -1)
            {
                L.append(i);
            }
            else
            {
                L.insertBefore(i);
            }
        }
        
        L.moveFront();
        while (L.index() >= 0)
        {
            out.println(stringArray[L.get()]);
            L.moveNext();
        }
   
        in1.close();
        in2.close();
        out.close();
    }
}