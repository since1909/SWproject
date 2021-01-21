#include <cassert>
#include "dnode.h"
#include "dnode.cpp"
#include "deque.h"

template <class Item>
deque<Item>::deque()
{

	front_ptr = NULL;
	back_ptr = NULL;
}

template <class Item>
deque<Item>::deque(const deque<Item>& source)
{
	list_copy(source.front_ptr, front_ptr, back_ptr);
}

template <class Item>
void deque<Item>::push_front(const Item& entry)
{
	if (empty())
	{
		front_ptr = new dnode<Item>(entry, front_ptr, NULL);
		back_ptr = front_ptr;
	}
	else
		list_head_insert(front_ptr, entry);
}

template <class Item>
void deque<Item>::push_back(const Item& entry)
{
	if (empty())
	{
		front_ptr = new dnode<Item>(entry, front_ptr, NULL);
		back_ptr = front_ptr;
	}
	else
		list_tail_insert(back_ptr, entry);
}

template <class Item>
void deque<Item>::pop_front()
{
	assert(!empty());
	list_head_remove(front_ptr);
}

template <class Item>
void deque<Item>::pop_back()
{
	assert(!empty());
	list_tail_remove(back_ptr);
}

template <class Item>
void deque<Item>::operator =(const deque<Item>& source)
{
	if (this == &source) // Handle self-assignment
		return;

	list_clear(front_ptr);
	list_copy(source.front_ptr, front_ptr, back_ptr);
}

template <class Item>
Item deque<Item>::front() const
{
	assert(!empty());
	return front_ptr->data();
}

template <class Item>
Item deque<Item>::back() const
{
	assert(!empty());
	return back_ptr->data();
}