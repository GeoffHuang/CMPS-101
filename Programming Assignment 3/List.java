//////////////////////////////////////
//||  Geoff Huang (ghuang6)       ||//
//||  CMPS 101 Professor Tantalo  ||//
//||  Programming Assignment 3    ||//
//////////////////////////////////////

// ===================================================================================
// List.java is a doubly linked list ADT with Objects in the data fields of the nodes.
// I simply modified my List.java from pa1 to accomodate for objects instead of ints.
// List of changes (pun intended):
// - I changed the data field in the Node class from int to Object
// - I changed the equals method in the Node class to use .equals() instead of ==
// - I changed the return type of front(), back(), get() functions from int to Object
// - I changed the parameter of the prepend(), append(), insertBefore(), and  
//   insertAfter() functions from int to Object
// ===================================================================================

class List 
{
	private class Node
	{
		// Fields
		Object data;
		Node next;
		Node prev;
	  
		// Constructor
		Node(Object data) {this.data = data; next = null;}
	  
		// overrides Object's toString() method
		public String toString() 
		{ 
			return data.toString(); 
		}
	  
		// overrides Object's equals() method
		public boolean equals(Object x)
		{
			boolean eq = false;
			Node that;
			if (x instanceof Node)
			{
				that = (Node) x;
				eq = (this.data.equals(that.data));
			}
			return eq;
		}
	}

	// Fields
	private Node front;
	private Node back;
	private Node cursor;
	private int length;
	private int index;

	// Constructor
	// Creates a new empty list
	List() 
	{ 
		front = back = cursor = null; 
		length = 0; 
		index = -1;
	}


	// Access Functions ==============================================================

	// Returns the number of elements in this List
	int length() 
	{ 
		return length; 
	}

	// index()
	// If cursor is defined, returns the index of the cursor element 
	// Otherwise returns -1
	int index()
	{
		return index;
	}
   
	// Returns front element
	// Pre: length() > 0
	Object front()
	{
		if( length() <= 0 )
		{
			throw new RuntimeException(
			"List Error: front() called on empty List");
		}
		return front.data;
	}
   
	// Returns back element
	// Pre: length() > 0
	Object back()
	{
		if( length() <= 0 )
		{
			throw new RuntimeException(
			"List Error: back() called on empty List");
		}
		return back.data;
	}
   
	// Returns cursor element
	// Pre: length() > 0, index() >= 0
	Object get()
	{
		if( length() <= 0)
		{
			throw new RuntimeException(
			"List Error: get() called on empty List");
		}
		if (index() < 0)
		{
			throw new RuntimeException(
			"List Error: get() called when the cursor is not pointing at anything");
		}
		return cursor.data;
	}
   
	// Overrides Object's equals() method.  Returns true if x is a List storing
	// the same integer sequence as this List; false otherwise.
	public boolean equals(Object x)
	{
		boolean eq  = false;
		List L;
		Node N, M;
		
		if (x instanceof List)
		{
			L = (List) x;
			N = this.front;
			M = L.front;
			eq = (this.length == L.length);
			while (eq && N != null)
			{
				eq = N.equals(M);
				N = N.next;
				M = M.next;
			}
		}
		return eq;
	}
	

	// Manipulation procedures========================================================
	
	// Resets this list to its original empty state
	void clear()
	{
		while (length() > 0)
		{
			deleteFront();
		}
	}
   
	// If List is non-empty, places the cursor under the front element
	// Otherwise, does nothing
	void moveFront()
	{
		if (length() > 0)
		{
			cursor = front;
			index = 0;
		}
	}
   
	// If List is non-empty, places the cursor under the back element
	// Otherwise, does nothing
	void moveBack()
	{
		if (length() > 0)
		{
			cursor = back;
			index = length() - 1;
		}
	}
   
	// If cursor is defined and not at front, moves cursor one step towards front
	// If cursor is defined and at front, cursor becomes undefined
	// If cursor is undefined, does nothing
	void movePrev()
	{
		if (cursor != null)
		{
			cursor = cursor.prev;
			index--;
		}
	}

	// If cursor is defined and not at back, moves cursor one step towards back
	// If cursor is defined and at back, cursor becomes undefined
	// If cursor is undefined, does nothing
	void moveNext()
	{
		if (cursor != null)
		{
			cursor = cursor.next;
			index++;
			if (index() > length() - 1)
			{
				index = -1;
			}
		}
	}
   
	// Insert new element into this List
	// If List is non-empty, insertion takes place before front element
	void prepend(Object data)
	{
		Node N = new Node(data);
		if (length() == 0) 
		{ 
			front = back = N;
		}
		else
		{ 
			front.prev = N; 
			N.next = front;
			front = N; 
		}
		if (index() >= 0)
		{
			index++;
		}
			length++;
	}

	// Insert new element into this List
	// If List is non-empty, insertion takes place after back element
	void append(Object data)
	{
		Node N = new Node(data);
		if (length() == 0) 
		{ 
			front = back = N;
		}
		else
		{ 
			back.next = N; 
			N.prev = back;
			back = N; 
		}
		length++;
	}
	  
	// Insert new element before cursor
	// Pre: length() > 0, index >= 0
	void insertBefore(Object data)
	{
		if (length() <= 0)
		{
			throw new RuntimeException(
			"List Error: insertBefore() called on empty List");
		}
		if (index() < 0)
		{
			throw new RuntimeException(
			"List Error: insertBefore() called when the cursor is not pointing at anything");
		}
	  
		Node N = new Node(data);  
		N.prev = cursor.prev;
		
		N.next = cursor;
		cursor.prev = N;
			   
		index++;
		length++;
		
		if (cursor.equals(front))
		{
			front = N;
		}
		else
		{
			N.prev.next = N;
		}
	}
	  
	// Insert new element after cursor
	// Pre: length() > 0, index >= 0
	void insertAfter(Object data)
	{
		if (length() <= 0)
		{
			throw new RuntimeException(
			"List Error: insertAfter() called on empty List");
		}
		if (index() < 0)
		{
			throw new RuntimeException(
			"List Error: insertAfter() called when the cursor is not pointing at anything");
		}
	  
		Node N = new Node(data);        
		N.next = cursor.next;

		N.prev = cursor;
		cursor.next = N;
		
		length++;
		
		if (cursor.equals(back))
		{
			back = N;
		}
		else
		{
			N.next.prev = N;
		}
	}
	 
	// Deletes the front element
	// Pre: length() > 0
	void deleteFront()
	{
		if (length() <= 0)
		{
			throw new RuntimeException(
			"List Error: deleteFront() called on empty List");
		}
		
		if (index() <= 0)
		{
			cursor = null;
			index = -1;
		}
		else
		{
			index--;
		}
		
		if (length() == 1)
		{
			front = back = null;
		}
		else
		{
			front = front.next;
			front.prev = null;
		}
		length--;
	}
   
	// Deletes the back element
	// Pre: length() > 0
	void deleteBack()
	{
		if (length() <= 0)
		{
			throw new RuntimeException(
			"List Error: deleteBack() called on empty List");
		}
		
		if (index() == length() - 1)
		{
			cursor = null;
			index = -1;
		}
		
		if (length() == 1)
		{
			front = back = null;
		}     
		else
		{
			back = back.prev;
			back.next = null;
		}
		length--;
	}
	
	// Deletes cursor element, making cursor undefined
	// Pre: length() > 0, index() >= 0
	void delete()
	{
		if (length() <= 0)
		{
			throw new RuntimeException(
			"List Error: delete() called on empty List");
		}
		if (index() < 0)
		{
			throw new RuntimeException(
			"List Error: delete() called when the cursor is not pointing at anything");
		}
		
		if (cursor.equals(front))
		{
			deleteFront();
		}
		else if (cursor.equals(back))
		{
			deleteBack();
		}
		else
		{
			cursor.prev.next = cursor.next;
			cursor.next.prev = cursor.prev;
			length--;
		}
		
		cursor = null;
		index = -1;
	}


	// Other functions ===============================================================

	// Overides Object's toString() method.
	public String toString()
	{
		StringBuffer sb = new StringBuffer();
		Node N = front;
		while (N != null)
		{
			sb.append(N.toString());
			// of course the TA wrote a check to make sure we don't have
			// an extra space at the end of our string :P
			if (N.next != null)
			{
				sb.append(" ");
			}
			N = N.next;
		}
		return new String(sb);
	}
}