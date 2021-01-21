#pragma once
#include <cstdlib>

template <class Item>
class dnode
{
public:
	// TYPEDEF
	typedef Item value_type;

	// CONSTRUCTOR
	dnode(const Item& init_data = Item( ),dnode* fore_init_link = NULL,dnode* back_init_link = NULL){
			data_field = init_data;
			fore_link_field = fore_init_link;
			back_link_field = back_init_link;
		}

    void set_data(const Item& new_data)	{ data_field = new_data; }
    void set_fore_link(dnode* new_fore_link)		{ fore_link_field = new_fore_link; }
	void set_back_link(dnode* new_back_link)	{ back_link_field = new_back_link; }

	Item& data( ) { return data_field; }

	const dnode* fore_link( ) const { return fore_link_field; }
   	dnode* fore_link( )             { return fore_link_field; }
	const dnode* back_link() const { return back_link_field; }
	dnode* back_link() { return back_link_field; }

private:
	Item data_field;
	dnode* fore_link_field;
	dnode* back_link_field;
};

// FUNCTIONS for the doubly linked list toolkit
template <class Item>
std::size_t list_length(const dnode<Item>* head_ptr);
 
template <class Item>
void list_head_insert(dnode<Item>*& head_ptr, const Item& entry);

template <class Item>
void list_tail_insert(dnode<Item>*& tail_ptr, const Item& entry);

template <class Item>
void list_insert(dnode<Item>* previous_ptr, const Item& entry);

template <class Item>
dnode<Item>* list_search(dnode<Item>* head_ptr, const Item& target);

template <class Item>
const dnode<Item>* list_search(const dnode<Item>* head_ptr, const Item& target);

template <class Item>
dnode<Item>* list_locate(dnode<Item>* head_ptr, std::size_t position);

template <class Item>
const dnode<Item>* list_locate(const dnode<Item>* head_ptr, std::size_t position);

template <class Item>
void list_head_remove(dnode<Item>*& head_ptr);

template <class Item>
void list_tail_remove(dnode<Item>*& tail_ptr);

template <class Item>
void list_remove(dnode<Item>* previous_ptr);

template <class Item>
void list_clear(dnode<Item>*& head_ptr);

template <class Item>
void list_copy(const dnode<Item>* source_ptr, dnode<Item>*& head_ptr, dnode<Item>*& tail_ptr);
