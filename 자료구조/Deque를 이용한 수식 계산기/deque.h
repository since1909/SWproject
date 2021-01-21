#pragma once
#include <cstdlib>
#include <cassert>
#include "dnode.h"

using namespace std;

template <class Item>
class deque
{
public:
	typedef std :: size_t size_type;
	typedef Item value_type;
	deque();
	deque(const deque& source);
	~deque<Item>() { front_ptr = NULL; back_ptr = NULL;}

	void push_front(const Item& entry);
	void push_back(const Item& entry);
	void pop_front();
	void pop_back();

	void operator = (const deque& source);

	Item front() const;
	Item back() const;

	bool empty() const { return ((front_ptr == NULL) || (back_ptr == NULL)); }

private:
	dnode<Item> *front_ptr;
	dnode<Item> *back_ptr;
};


