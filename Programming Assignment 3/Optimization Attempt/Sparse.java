//////////////////////////////////////
//||  Geoff Huang (ghuang6)       ||//
//||  CMPS 101 Professor Tantalo  ||//
//||  Programming Assignment 3    ||//
//////////////////////////////////////

// ===================================================================================
// Sparse.java is the top level client module for programming assignment 3.
// It takes two command line arguments - an infile name and an outfile name.
// The infile must be structured a certain way to specify the contents of two square
// matrices A and B. Using Matrix.java and List.java, I then compute certain specified
// matrix operations and write the results to the outfile.
// ===================================================================================

import java.io.*;
import java.util.Scanner;

class Sparse
{
	public static void main(String[] args) throws IOException
	{
		int line = 1;
		int n, A_NNZ, B_NNZ;
		Scanner in = new Scanner(new File(args[0]));
		PrintWriter out = new PrintWriter(new FileWriter(args[1]));
		Matrix A, B;

		// Check that there are two command line arguments
		// Quit with a usage message to stderr if not
		if (args.length != 2)
		{
			System.err.println("Please enter only two arguments");
			System.err.println("Usage: FileIO infile outfile");
			System.exit(1);
		}

		// Grab the first line of the input file
		n = in.nextInt();	// Matrix size
		A = new Matrix(n);
		B = new Matrix(n);
		A_NNZ = in.nextInt();	// Matrix A number of nonzero values
		B_NNZ = in.nextInt();	// Matrix B number of nonzero values

		// Check for invalid output (next two tokens must be newlines)
		for (int i = 0; i < 2; i++)
		{
			if (!in.nextLine().equals(""))
			{
				System.err.println("Invalid input at line " + line + " of " + args[0]);
				System.exit(1);
			}
			line++;
		}

		// Grab the next chunk of data and insert it into Matrix A
		for (int i = 0; i < A_NNZ; i++)
		{
			int row = in.nextInt();
			int col = in.nextInt();
			double val = in.nextDouble();

			if (!in.nextLine().equals(""))
			{
				System.err.println("Invalid input at line " + line + " of " + args[0]);
				System.exit(1);
			}

			A.changeEntry(row, col, val);
			line++;
		}
		
		// Check for newline
		if (!in.nextLine().equals(""))
		{
			System.err.println("Invalid input at line " + line + " of " + args[0]);
			System.exit(1);
		}
		line++;

		// Grab the next chunk of data and insert it into Matrix B
		for (int i = 0; i < B_NNZ; i++)
		{
			int row = in.nextInt();
			int col = in.nextInt();
			double val = in.nextDouble();

			if (!in.nextLine().equals(""))
			{
				System.err.println("Invalid input at line " + line + " of " + args[0]);
				System.exit(1);
			}
			
			B.changeEntry(row, col, val);
			line++;
		}

		//////////////////////////
		//|| Write to outfile ||//
		//////////////////////////

		// Matrix A contents
		out.println("A has " + A_NNZ + " non-zero entries:");
		out.println(A.toString());

		// Matrix B contents
		out.println("B has " + B_NNZ + " non-zero entries:");
		out.println(B.toString());
				
		// (1.5)*A
		out.println("(1.5)*A = ");
		out.println(A.scalarMult(1.5).toString());

		// A+B
		out.println("A+B = ");
		out.println(A.add(B).toString());

		// A+A
		out.println("A+A = ");
		out.println(A.add(A).toString());

		// B-A
		out.println("B-A = ");
		out.println(B.sub(A).toString());

		// A-A
		out.println("A-A = ");
		out.println(A.sub(A).toString());

		// Transpose(A)
		out.println("Transpose(A) = ");
		out.println(A.transpose().toString());

		// A*B
		out.println("A*B = ");
		out.println(A.mult(B).toString());

		// B*B
		out.println("B*B = ");
		out.println(B.mult(B).toString());

		in.close();
		out.close();
	}
}