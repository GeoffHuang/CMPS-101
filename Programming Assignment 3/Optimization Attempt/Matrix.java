//////////////////////////////////////
//||  Geoff Huang (ghuang6)       ||//
//||  CMPS 101 Professor Tantalo  ||//
//||  Programming Assignment 3    ||//
//////////////////////////////////////

// ===================================================================================
// Matrix.java is a matrix implementation where each row in the Matrix is a List of
// Objects, where each Object is of class Entry.
// As a result, Matrix.java is meant to be used alongside the List.java that I wrote.
// This implementation can perform basic matrix operations much faster on sparse
// matrices (matrices with a small number of non-zero entries) than a matrix written
// as a two dimensional array.
// ===================================================================================

class Matrix
{
	private class Entry
	{
		// Entry Class Fields
		int col;	// column of the matrix
		double val; // value of the matrix entry

		// Entry constructor
		Entry(int col, double val)
		{
			this.col = col;
			this.val = val;
		}

		// overrides Object's toString() method
		public String toString() 
		{ 
			return "("+col+", "+val+")";
			// returns (col, val)
		}

		// overrides Object's equals() method
		public boolean equals(Object x)
		{
			boolean eq = false;
			Entry that;
			if (x instanceof Entry)
			{
				that = (Entry) x;
				eq = (this.col == that.col && this.val == that.val);
			}
			return eq;
		}
	}

	// Matrix Class Fields
	List[] row;	// each row is a List

	// Matrix Constructor
	Matrix(int n)
	{
		if (n < 1)
			throw new RuntimeException(
			"Matrix Error: Cannot construct an "+n+"x"+n+" matrix.");

		row = new List[n+1]; // I am not using the 0th row
		for (int i = 1; i < n+1; i++)
		{
			row[i] = new List();
		}

	}

	// Access Functions ==============================================================

	// returns n, the number of rows and columns of this Matrix
	int getSize()
	{
		return (row.length - 1);
	}

	// returns the number of non-zero entries in this Matrix
	int getNNZ()
	{
		int NNZ = 0;
		for (int i = 1; i <= getSize(); i++)
		{
			NNZ += row[i].length();
		}
		return NNZ;
	}

	// overrides Object's equals() method
	public boolean equals(Object x)
	{
		boolean eq  = false;
		Matrix M;
		int index = 1;

		if (x instanceof Matrix)
		{
			M = (Matrix) x;
			eq = (this.getSize() == M.getSize());
			while (eq && index <= this.getSize())
			{
				eq = row[index].equals(M.row[index]);
				index++;
			}
		}
		return eq;
	}

	// Manipulation procedures========================================================

	// sets this Matrix to the zero state
	void makeZero()
	{
		for (int i = 1; i <= getSize(); i++)
		{
			row[i].clear();
		}
	}

	// returns a new Matrix having the same entries as this Matrix
	Matrix copy()
	{
		Matrix M = new Matrix(getSize());

		for (int i = 1; i <= getSize(); i++)
		{
			row[i].moveFront();
			while (row[i].index() >= 0)
			{
				M.changeEntry(i, ((Entry)row[i].get()).col, ((Entry)row[i].get()).val);
				row[i].moveNext();
			}			
		}

		return M;
	}

	// changes ith row, jth column of this Matrix to x
	// pre: 1<=i<=getSize(), 1<=j<=getSize()
	void changeEntry(int i, int j, double x)
	{
		if (i < 1 || i > getSize() || j < 1 || j > getSize())
		{
			throw new RuntimeException(
			"Matrix error: changeEntry() called with row/col arguments that are out of bounds.");
		}

		// search through the row's list
		row[i].moveFront();
		while (row[i].index() >= 0)
		{
			// if the column contains a non zero entry
			if (((Entry)row[i].get()).col == j)
			{
				if (x == 0)
				{
					// delete if you're changing to zero entry
					row[i].delete();
				}
				else
				{
					// else change entry value
					((Entry)row[i].get()).val = x;
				}
				return;
			}
			// if the column contains a zero entry (column not found)
			if (((Entry)row[i].get()).col > j)
			{
				if (x != 0)
				{
					row[i].insertBefore(new Entry(j,x));
				}
				return;
				// Note: if x == 0, don't need to do anything   		
			}
			row[i].moveNext();
		}

		// if the entry was not changed in the above while loop,
		// it must have a higher column value than all existing entries in that row
		if (x != 0)
		{
			row[i].append(new Entry(j,x)); // so append it
		}
	}

	// returns a new Matrix that is the scalar producxt of this Matrix with x
	Matrix scalarMult(double x)
	{
		Matrix M = new Matrix(getSize());

		for (int i = 1; i <= getSize(); i++)
		{
			row[i].moveFront();
			while (row[i].index() >= 0)
			{
				M.changeEntry(i, ((Entry)row[i].get()).col, ((Entry)row[i].get()).val * x);
				row[i].moveNext();
			}			
		}

		return M;
	}

	// returns a new Matrix that is the sum of this Matrix with M
	// pre: getSize() == M.getSize()
	// make copy, add in other matrix
	Matrix add(Matrix M)
	{
		if (getSize() != M.getSize())
		{
			throw new RuntimeException(
			"Matrix Error: Cannot add two matrices of different dimensions.");
		}

		Matrix N = M.copy();	// for cases where you add a matrix to itself
		Matrix Sum = new Matrix(getSize());

		for (int i = 1; i <= getSize(); i++)
		{
			row[i].moveFront();
			N.row[i].moveFront();

			while (row[i].index() >= 0 && N.row[i].index() >= 0)
			{
				// for readability
				Entry E1 = (Entry)row[i].get();
				Entry E2 = (Entry)N.row[i].get();

				if (E1.col < E2.col)
				{
					Sum.changeEntry(i, E1.col, E1.val);
					row[i].moveNext();
				}
				else if (E1.col == E2.col)
				{
					Sum.changeEntry(i, E1.col, E1.val + E2.val);
					row[i].moveNext();
					N.row[i].moveNext();
				}
				else if (E1.col > E2.col)
				{
					Sum.changeEntry(i, E2.col, E2.val);
					N.row[i].moveNext();
				}
			}

			if (row[i].index() >= 0)
			{
				while (row[i].index() >= 0)
				{
					Entry E1 = (Entry)row[i].get();
					Sum.changeEntry(i, E1.col, E1.val);
					row[i].moveNext();
				}
			}
			else if (N.row[i].index() >= 0)
			{
				while (N.row[i].index() >= 0)
				{
					Entry E2 = (Entry)N.row[i].get();
					Sum.changeEntry(i, E2.col, E2.val);
					N.row[i].moveNext();
				}
			}	
		}

		return Sum;
	}

	// returns a new Matrix that is the difference of this Matrix with M
	// pre: getSize() == M.getSize()
	Matrix sub(Matrix M)
	{
		if (getSize() != M.getSize())
		{
			throw new RuntimeException(
			"Matrix Error: Cannot subtract two matrices of different dimensions.");
		}

		Matrix N = M.copy();	// for cases where you subtract a matrix from itself
		Matrix Difference = new Matrix(getSize());

		for (int i = 1; i <= getSize(); i++)
		{
			row[i].moveFront();
			N.row[i].moveFront();

			while (row[i].index() >= 0 && N.row[i].index() >= 0)
			{
				// for readability
				Entry E1 = (Entry)row[i].get();
				Entry E2 = (Entry)N.row[i].get();

				if (E1.col < E2.col)
				{
					Difference.changeEntry(i, E1.col, E1.val);
					row[i].moveNext();
				}
				else if (E1.col == E2.col)
				{
					Difference.changeEntry(i, E1.col, E1.val - E2.val);
					row[i].moveNext();
					N.row[i].moveNext();
				}
				else if (E1.col > E2.col)
				{
					Difference.changeEntry(i, E2.col, 0 - E2.val);
					N.row[i].moveNext();
				}
			}

			if (row[i].index() >= 0)
			{
				while (row[i].index() >= 0)
				{
					Entry E1 = (Entry)row[i].get();
					Difference.changeEntry(i, E1.col, E1.val);
					row[i].moveNext();
				}
			}
			else if (N.row[i].index() >= 0)
			{
				while (N.row[i].index() >= 0)
				{
					Entry E2 = (Entry)N.row[i].get();
					Difference.changeEntry(i, E2.col, 0 - E2.val);
					N.row[i].moveNext();
				}
			}	
		}

		return Difference;
	}

	// returns a new Matrix that is the transpose of this Matrix
	Matrix transpose()
	{
		Matrix M = new Matrix(getSize());

		for (int i = 1; i <= getSize(); i++)
		{
			row[i].moveFront();
			while (row[i].index() >= 0)
			{
				M.changeEntry(((Entry)row[i].get()).col, i, ((Entry)row[i].get()).val);
				row[i].moveNext();
			}
		}

		return M;
	}

	// returns a new Matrix that is the product of this Matrix with M
	// pre: getSize() == M.getSize()
	Matrix mult(Matrix M)
	{
		if (getSize() != M.getSize())
		{
			throw new RuntimeException(
			"Matrix Error: Cannot multiply two square matrices of different dimensions.");
		}

		Matrix Product = new Matrix(getSize());
		Matrix MT = M.transpose();

		// iterating through this Matrice's rows
		for (int i = 1; i <= getSize(); i++)
		{
			// iterating through M's columns (aka MT's rows)
			for (int j = 1; j <= getSize(); j++)
			{
				row[i].moveFront();
				MT.row[j].moveFront();

				double dotprod = 0;

				while (row[i].index() >= 0 && MT.row[j].index() >= 0)
				{
					// for readability
					Entry E1 = (Entry)row[i].get();
					Entry E2 = (Entry)MT.row[j].get();

					if (E1.col < E2.col)
					{
						row[i].moveNext();
					}
					else if (E1.col == E2.col)
					{
						// only need to take dot product if both entries are nonzero
						dotprod += (E1.val * E2.val);
						row[i].moveNext();
						MT.row[j].moveNext();
					}
					else if (E1.col > E2.col)
					{
						MT.row[j].moveNext();
					}
				}

				Product.changeEntry(i, j, dotprod);
			}
		}

		return Product;
	}


	// Other functions ===============================================================

	// overrides Object's toString() method
	public String toString()
	{
		StringBuffer sb = new StringBuffer();

		for (int i = 1; i <= getSize(); i++)
		{
			row[i].moveFront();

			// row number - "i: "
			if (row[i].length() != 0)
			{
				sb.append(String.valueOf(i));
				sb.append(": ");
				sb.append(row[i].toString());
				sb.append("\n");
			}
		}
		return new String(sb);
	}
}

