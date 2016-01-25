#ifndef CUSTOM_STRUCTURES_H
#define CUSTOM_STRUCTURES_H
#pragma warning(disable:4995)

//
// List struct and methods
//
// Custom List to hold class pointer
// Does not currently de-construct object from held pointer

template <class T> struct Node

{
	private:

		Node* previous;
		Node* next;

	public:

		T* data;
};

template <class T> struct Custom_List
{
private:

	Node<T>* start_ptr;
	Node<T>* end_ptr; // Pointer to the last node

public:
	
	
	struct Custom_Iterator
	{
	private:

		Node* current_node;

	public:
		
		Custom_Iterator();

		Custom_Iterator& operator=( Custom_Iterator &RHS ) const
		{
			current_node = RHS.current_node;

			return *this;
		}

		Custom_Iterator& operator++ () const
		{
			current_node = current_node->next;
			return *this; 
		}
		Custom_Iterator operator++ (int) const
		{
			Custom_Iterator temp = *this;
			current_node = current_node->next;
			return temp;
		}

		Custom_Iterator& operator -- () const
		{
			current_node = current_node->previous;
			return *this; 
		}
		Custom_Iterator operator-- (int) const
		{
			Custom_Iterator temp = *this;
			current_node = current_node->previous;
			return temp;
		}


	};

	Custom_List();	// Default List
	~Custom_List();

	//methods
	Node<T>*		Begin()		{ return start_ptr };
	Node<T>*		End()		{ return end_ptr };

	void		pop_back();

	void		push_back(const T* service);

	void		clear();

};


template <class T> Custom_List<T>::Custom_Iterator::Custom_Iterator()
{
	current_node = NULL;
}

template <class T> Custom_List<T>::Custom_List()

{
	end_ptr = start_ptr = NULL;
}

template <class T> Custom_List<T>::~Custom_List()

{
	clear();
}


template <class T> void Custom_List<T>::pop_back()
{

	end_ptr = end_ptr.previous;
	delete end_ptr.next;

}

template <class T> void Custom_List<T>::push_back(const T* service)
{

	Node<T> newnode = new Node;
	newnode.next = NULL;
	newnode.previous = NULL;
	newnode.data = service;

	if (start_ptr == NULL)
	{
		start_ptr = newnode;
		end_ptr = newnode;
	}
	else
	{
		newnode.previous = end_ptr;
		end_ptr->next = newnode;
		end_ptr = newnode;
	}

}

template <class T> void Custom_List<T>::clear()
{

	while (end_ptr != NULL)
	{
		end_ptr = end_ptr->previous;
		delete end_ptr->next;

	}

	delete start_ptr;
	start_ptr = NULL;
	end_ptr = NULL;

}

#endif

