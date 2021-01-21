#include "dnode.h"
#include <cassert>    // Provides assert
#include <cstdlib>
#include <iostream>
using namespace std;

template <class Item>
size_t list_length(const dnode<Item>* head_ptr)
{
	dnode<Item> *cursor;
	size_t answer;

	answer = 0;
	for (cursor = head_ptr; cursor != NULL; cursor = cursor->fore_link())
		++answer;

	return answer;
}

template <class Item>
void list_head_insert(dnode<Item>*& head_ptr, const Item& entry)
{
	head_ptr = new dnode<Item>(entry, head_ptr, NULL);
    head_ptr->fore_link()->set_back_link(head_ptr);
}


template <class Item>
void list_tail_insert(dnode<Item>*& tail_ptr, const Item& entry)
{
	tail_ptr = new dnode<Item>(entry, NULL,tail_ptr);
	tail_ptr->back_link()->set_fore_link(tail_ptr);
}


template <class Item>
void list_insert(dnode<Item>* previous_ptr, const Item& entry)
{
	dnode<Item>* insert_ptr;
    
	insert_ptr = new dnode<Item>(entry, previous_ptr->fore_link( ), previous_ptr);
	previous_ptr->set_fore_link(insert_ptr);
	insert_ptr->back_link(insert_ptr);
	//previous_ptr->fore_link()->set_back_link(insert_ptr->back_link());
	//insert_ptr->set_back_link(insert_ptr);
}

template <class Item>
dnode<Item>* list_search(dnode<Item>* head_ptr, const Item& target)
// Library facilities used: cstdli
{
	dnode<Item> *cursor;
   
	for (cursor = head_ptr; cursor != NULL; cursor = cursor->fore_link( ))
	    if (target == cursor->data( ))
		return cursor;
	return NULL;
}

template <class Item>
const dnode<Item>* list_search(const dnode<Item>* head_ptr, const Item& target)
// Library facilities used: cstdlib
{
	const dnode<Item> *cursor;
   
	for (cursor = head_ptr; cursor != NULL; cursor = cursor->fore_link( ))
	    if (target == cursor->data( ))
		return cursor;
	return NULL;
}

template <class Item>
dnode<Item>* list_locate(dnode<Item>* head_ptr, size_t position)
// Library facilities used: cassert, cstdlib
{
	dnode<Item> *cursor;
	size_t i;
    
	assert (0 < position);
	cursor = head_ptr;
	for (i = 1; (i < position) && (cursor != NULL); i++)
	    cursor = cursor->fore_link( );
	return cursor;
}

template <class Item>
const dnode<Item>* list_locate(const dnode<Item>* head_ptr, size_t position)
// Library facilities used: cassert, cstdlib
{
	const dnode<Item> *cursor;
	size_t i;
    
	assert (0 < position);
	cursor = head_ptr;
	for (i = 1; (i < position) && (cursor != NULL); i++)
	    cursor = cursor->fore_link( );
	return cursor;
}

template <class Item>
void list_head_remove(dnode<Item>*& head_ptr)
{
	dnode<Item> *remove_ptr = head_ptr;

	if (remove_ptr != NULL) {
		head_ptr = head_ptr->fore_link();
		if ((remove_ptr->fore_link() == NULL) && (remove_ptr->back_link() == NULL))
		{
			delete remove_ptr;
			remove_ptr = nullptr;
		}
		else
		{
			remove_ptr->set_fore_link(NULL);
			head_ptr->set_back_link(NULL);
			delete remove_ptr;
			remove_ptr = nullptr;
		}
	}
}

template <class Item>
void list_tail_remove(dnode<Item>*& tail_ptr)
{
	dnode<Item> *remove_ptr = tail_ptr;

	if (remove_ptr != NULL) {
		tail_ptr = tail_ptr->back_link();
		if ((remove_ptr->fore_link() == NULL) && (remove_ptr->back_link() == NULL))
			delete remove_ptr;
		else
		{
			remove_ptr->set_back_link(NULL);
			tail_ptr->set_fore_link(NULL);
			delete remove_ptr;
		}
	}
}
template <class Item>
void list_remove(dnode<Item>* previous_ptr)
{
	dnode<Item> *remove_ptr;

	remove_ptr = previous_ptr->fore_link( );
	previous_ptr->set_fore_link( remove_ptr->fore_link( ) );
	remove_ptr->set_back_link(previous_ptr);
	delete remove_ptr;
}

template <class Item>
void list_clear(dnode<Item>*& head_ptr)
{
	while (head_ptr != NULL)
	    list_head_remove(head_ptr);
    
}

template <class Item>
void list_copy(dnode<Item>* source_ptr, dnode<Item>*& head_ptr, dnode<Item>*& tail_ptr)
{
	head_ptr = NULL;
	tail_ptr = NULL;

	// Handle the case of the empty list.
	if (source_ptr == NULL)
	    return;
    
	// Make the head node for the newly created list, and put data in it.
	head_ptr = new dnode<Item>(source_ptr->data(), head_ptr, tail_ptr);
	tail_ptr = head_ptr;
    
	// Copy the rest of the nodes one at a time, adding at the tail of new list.
	source_ptr = source_ptr->fore_link(); 
	while (source_ptr != NULL)
	{
	    //list_insert(tail_ptr, source_ptr->data( ));
		list_tail_insert(tail_ptr, source_ptr->data());
	    source_ptr = source_ptr->fore_link();
	}
}
