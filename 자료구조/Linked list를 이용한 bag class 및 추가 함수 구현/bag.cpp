// FILE: bag3.cxx
// CLASS implemented: bag (see bag3.h for documentation)
// INVARIANT for the bag ADT:
//   1. The items in the bag are stored on a linked list;
//   2. The head pointer of the list is stored in the member variable head_ptr;
//   3. The total number of items in the list is stored in the member variable
//      many_nodes.

#include <iostream>
#include <cassert>  // Provides assert
#include <cstdlib>  // Provides NULL, rand, size_t
#include "node.h"  // Provides node and the linked list functions
#include "bag.h"
using namespace std;

namespace main_savitch_5
{

    bag::bag( )
    // Library facilities used: cstdlib
    {
	head_ptr = NULL;
	many_nodes = 0;
    }

    bag::bag(const bag& source)
    // Library facilities used: node1.h
    {
        node *tail_ptr;  // Needed for argument of list_copy

        list_copy(source.head_ptr, head_ptr, tail_ptr);
        many_nodes = source.many_nodes;
    }
    
    bag::~bag( )
    // Library facilities used: node1.h
    {
        list_clear(head_ptr);
        many_nodes = 0;
    }

    bag::size_type bag::count(const value_type& target) const
    // Library facilities used: cstdlib, node1.h
    {
	size_type answer;
	const node *cursor; // Use const node* since we don't change the nodes.
	
	answer = 0;
	cursor = list_search(head_ptr, target);
	while (cursor != NULL)
	{
	    // Each time that cursor is not NULL, we have another occurrence of
	    // target, so we add one to answer, and move cursor to the next
	    // occurrence of the target.
	    ++answer;
	    cursor = cursor->link( );
	    cursor = list_search(cursor, target);
	}
	return answer;
    }

    bag::size_type bag::erase(const value_type& target)
    // Library facilities used: cstdlib, node1.h
    {
        size_type answer = 0;
        node *target_ptr;

        target_ptr = list_search(head_ptr, target);
        while (target_ptr != NULL)
        {
            // Each time that target_ptr is not NULL, we have another occurrence
	    // of target. We remove this target using the same technique that
	    // was used in erase_one.
            target_ptr->set_data( head_ptr->data( ) );
            target_ptr = target_ptr->link( );
            target_ptr = list_search(target_ptr, target);
            list_head_remove(head_ptr);
	    --many_nodes;
            ++answer;
        }
        return answer;
    }
    
    bool bag::erase_one(const value_type& target)
    // Library facilities used: cstdlib, node1.h
    {
	node *target_ptr;
	
	target_ptr = list_search(head_ptr, target);
	if (target_ptr == NULL)
	    return false; // target isn't in the bag, so no work to do
	target_ptr->set_data( head_ptr->data( ) );
	list_head_remove(head_ptr);
	--many_nodes;
	return true;
    }

	//HOMEWORK_1
	void bag::removeRepetition()
	{
		node* cursor;
		cursor = head_ptr;
		while (cursor->link() != NULL)
		{
			if (list_search(cursor->link(), cursor->data()) == NULL)
				cursor = cursor->link();
			else
				list_remove(cursor);
		}
	}

	void bag::reverse()
	{
		node* first_ptr;
		node* cursor;
		first_ptr = head_ptr;
		cursor = head_ptr->link();
		while (cursor != NULL)
		{
			list_head_insert(head_ptr, cursor->data());
			list_remove(first_ptr);
			cursor = first_ptr->link();
		}
	}

	void bag::sort()
	{		
		node* cursor1;
		node* cursor2;
		size_t temp;
		for (cursor1 = head_ptr; cursor1 != NULL; cursor1 = cursor1->link())
		{
			for (cursor2 = cursor1; cursor2 != NULL; cursor2 = cursor2->link())
			{
				if (cursor1->data() > cursor2->data())
				{
					temp = cursor2->data();
					cursor2->set_data(cursor1->data());
					cursor1->set_data(temp);
				}
			}
		}
	}

	void bag::show_content()
	{
		node* cursor;
		for (cursor = head_ptr; cursor != NULL; cursor = cursor->link())
			cout << cursor->data() << '\n';
	}

    bag::value_type bag::grab( ) const
    // Library facilities used: cassert, cstdlib, node1.h
    {
	size_type i;
	const node *cursor; // Use const node* since we don't change the nodes.

	assert(size( ) > 0);
	i = (rand( ) % size( )) + 1;
	cursor = list_locate(head_ptr, i);
	return cursor->data( );
    }

    void bag::insert(const value_type& entry)
    // Library facilities used: node1.h
    {
	list_head_insert(head_ptr, entry);
	++many_nodes;
    }

    void bag::operator +=(const bag& addend)
    // Library facilities used: cstdlib, node.h
    {
	node *copy_head_ptr;
	node *copy_tail_ptr;
	
	if (addend.many_nodes > 0)
	{
	    list_copy(addend.head_ptr, copy_head_ptr, copy_tail_ptr);
	    copy_tail_ptr->set_link( head_ptr ); 
	    head_ptr = copy_head_ptr;
	    many_nodes += addend.many_nodes;
	}
    }

	void bag::operator -=(const bag& subtract)
	//HOMEWORK_1
	{
		node* cursor;
		for (cursor = subtract.head_ptr; cursor != NULL; cursor = cursor->link())
		{
			if (list_search(head_ptr, cursor->data()) != NULL)
				erase_one(list_search(head_ptr, cursor->data())->data());
		}
	}
    
    void bag::operator =(const bag& source)
    // Library facilities used: node1.h
    {
	node *tail_ptr; // Needed for argument to list_copy

	if (this == &source)
            return;

	list_clear(head_ptr);
	many_nodes = 0;
	list_copy(source.head_ptr, head_ptr, tail_ptr);
	many_nodes = source.many_nodes;
    }
	
	bool bag::operator ==(const bag& source)
	//HOMEWORK_1
	{
		node* cursor;
		if (this->many_nodes != source.many_nodes)
			return false;
		for (cursor = source.head_ptr; cursor != NULL; cursor = cursor->link())
		{
			if (list_search(head_ptr, cursor->data()) == NULL)
				return false;
		}
		return true;

	}

    bag operator +(const bag& b1, const bag& b2)
    {
	bag answer;

	answer += b1; 
	answer += b2;
	return answer;
    }

	//HOMEWORK_1
	bag operator -(bag& b1, bag& b2)
	{
		b1 -= b2;
		return b1;
	}
}
