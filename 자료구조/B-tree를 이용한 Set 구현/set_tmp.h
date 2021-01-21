#include <cstdlib>   // Provides size_t
#include <iomanip>
#include <iostream>
using namespace std;

template <class Item>
class set
{	
public:
	typedef Item value_type;
	
	set<Item>() {
		data_count = 0;
		child_count = 0;
		root_ptr = NULL;
	}
	set<Item>(set& source) {
		this->root_ptr = source.root_ptr;
	}
	~set() { clear(); }
	
	// MODIFICATION MEMBER FUNCTIONS
	void operator =(const set& source) {
		this->root_ptr = source.root_ptr;
	}
	void clear(){
		root_ptr = NULL;
		data_count = 0;
		child_count = 0;
	}
	bool insert(const Item& entry);
	std::size_t erase(const Item& target);
	
	// CONSTANT MEMBER FUNCTIONS
	std::size_t count(const Item& target) const;

	bool empty() const { return (data_count == 0); }
	
	// SUGGESTED FUNCTION FOR DEBUGGING
	void show_content();
private:
	// MEMBER CONSTANTS
	static const std::size_t MINIMUM = 1;
	static const std::size_t MAXIMUM = 2 * MINIMUM;
	
	// MEMBER VARIABLES

	std::size_t data_count;
	Item data[MAXIMUM + 1];
	std::size_t child_count;
	set *subset[MAXIMUM + 2];
	set<Item> *root_ptr;
	// HELPER MEMBER FUNCTIONS
	bool is_leaf() const { return (child_count == 0); }
	bool loose_insert(const Item& entry);
	bool loose_erase(const Item& target);
	void remove_biggest(Item& removed_entry);
	void fix_excess(std::size_t i);
	void fix_shortage(std::size_t i);
	// NOTE: The implementor may want to have additional helper functions
	void print_tree(int depth);
	std::size_t counter(const Item& target) const;
};

template <class Item>
void set<Item>::show_content() 
{
	if (root_ptr != NULL)
		root_ptr->print_tree(0);
}

template <class Item>
void set<Item>::print_tree(int depth) {
	int i;
	if (!is_leaf())
	{
		for (i = child_count - 1; i >= 0; i--)
		{
			subset[i]->print_tree(depth + 2);
			if (i != 0)
			{
				cout << setw(4 * depth) << " " << data[i - 1] << endl;
			}
		}
	}
	else
	{
		for (i = data_count - 1; i >= 0; i--)
			cout << setw(4 * depth) << " " << data[i] << endl;
	}
}

template <class Item>
std::size_t set<Item>::count(const Item& target) const {
	size_t result;
	if (root_ptr != NULL)
		result = root_ptr->counter(target);
	return result;
}

template <class Item>
std::size_t set<Item>::counter(const Item& target) const {
	
	int i = 0;
	while (i < data_count && data[i] <= target)
		i++;

	if (data[i-1] == target)
		return 1;
	else if (is_leaf())
		return 0;
	else
		subset[i]->counter(target);

}

template <class Item>
void set<Item>::fix_excess(std::size_t i) {
	int index;
	set<Item> *left = new set<Item>;
	set<Item> *right = new set<Item>;
	for (index = 0; index < MINIMUM; index++)
	{
		left->data[index] = subset[i]->data[index];
		left->data_count++;
		right->data[index] = subset[i]->data[index + MINIMUM + 1];
		right->data_count++;
	} //split
	if (!(subset[i]->is_leaf())) {
		for (index = 0; index <= MINIMUM; index++)
		{
			left->subset[index] = subset[i]->subset[index];
			left->child_count++;
			right->subset[index] = subset[i]->subset[index + MINIMUM + 1];
			right->child_count++;
		}
	}
	data[i] = subset[i]->data[MINIMUM];  //중간값 parent로 올림
	data_count++;
	subset[i] = left;
	subset[i + 1] = right;
	child_count++;
}

template <class Item>
bool set<Item>::loose_insert(const Item& entry) {
	int i = 0;
	while (i < data_count && data[i] <= entry)  //count 증가도 안되고
		i++;
	if (data[i-1] == entry)
		return false;
	else if (is_leaf())
	{
		//배열 길이 고려하기 빈자리 있나 확인
		int j = data_count;		//data_count -1

		while (j != i)
		{
			data[j] = data[j - 1];
			j--;
		} // 빈자리 만들기
		data[i] = entry;
		data_count++;
		return true;
	}
	else
	{
		bool b = subset[i]->loose_insert(entry);
		if (subset[i]-> data_count > MAXIMUM)
			fix_excess(i);
		return b;
	}
}

template <class Item>
bool set<Item>::insert(const Item& entry)
{
	if (root_ptr == NULL) {
		root_ptr = new set<Item>();
		root_ptr->data[0] = entry;
		root_ptr->data_count++;
	}
	else{
		if (!(root_ptr->loose_insert(entry)))
			return false;
		if (root_ptr->data_count > MAXIMUM)
		{
			set<Item> *new_root = new set<Item>();
			new_root->subset[0] = this->root_ptr;
			new_root->child_count++;
			new_root->fix_excess(0);
			root_ptr = new_root;
		}
	}
	return true;
}

template <class Item>
bool set<Item>::loose_erase(const Item& target)
{
	int i = 0;
	while (i< data_count && data[i] < target)
		i++;
	if (is_leaf() && i == data_count)
		return false;
	else if (is_leaf() && data[i] == target)
	{
		/*if (data_count == 1)
		{
			data[0] = NULL;
		}*/
		while (i < data_count-1)
		{
			data[i] = data[i + 1];
			i++;
		}
		data_count--;
		return true;
	}
	else //if(!is_leaf())
	{
		if (data[i] != target)
		{
			bool b = subset[i]->loose_erase(target);
			if (subset[i]->data_count == MINIMUM - 1)
				fix_shortage(i);
		}
		else
		{
			subset[i]->remove_biggest(data[i]);
			if (subset[i]->data_count < MINIMUM)
				fix_shortage(i);
			return true;
		}
	}
}

template <class Item>
void set<Item>::fix_shortage(std::size_t i) 
{
	int k;
	if (i != 0 && subset[i - 1]->data_count > MINIMUM)						// 앞/뒤에 노드가 최소값보다 많을 때 앞/뒤에서 가져다 사용
	{
		for (k = subset[i]->data_count; k > 0; k--)					//subset[i]->data에서 맨 앞에 자리 만들기
			subset[i]->data[k] = subset[i]->data[k - 1];
		subset[i] -> data[0] = data[i - 1];							//subset[i]의 맨 앞에 this의 데이터 맨 뒤에 값을 대입
		
		data[i - 1] = subset[i - 1]->data[subset[i - 1]->data_count - 1];
		subset[i - 1]->data_count--;								//this의 데이터 맨 뒤에 subset[i-1]의 마지막 값 대입 + data_count -1
		
		if (!subset[i - 1]->is_leaf())								//subset[i-1]이 leaf가 아니라면(child를 가지고 있다면)
		{
			for (k = subset[i]->child_count; k > 0; k--)			//subset[i]->child 에서 맨 앞에 자리 만들기
				subset[i]->subset[k] = subset[i]->subset[k - 1];
			subset[i]->subset[0] = subset[i - 1]->subset[subset[i-1]->child_count - 1];	//subset[i-1]의 맨 뒤 child를 subset[i]의 child 맨 앞으로 이동
			
			subset[i - 1]->child_count--;
			subset[i]->child_count++;
		}
	}
	else if (i < child_count-1 && subset[i + 1]->data_count > MINIMUM)
	{
		subset[i]->data[subset[i]->data_count] = data[i];
		subset[i]->data_count++;
		data[i] = subset[i + 1]->data[0];							//data[i]에 subset[i+1]의 맨 앞 값 대입 
		for (k = 1; k < subset[i + 1]->data_count; k++)				//subset[i+1]의 맨 앞자리 삭제
			subset[i + 1]->data[k - 1] = subset[i + 1]->data[k];
		subset[i + 1]->data_count--;								//data_count 1 감소
		
		if (!subset[i]->is_leaf())									//subset[i]가 leaf가 아닐때(child를 가지고 있을 때)
		{
			subset[i]->subset[child_count] = subset[i + 1]->subset[0];	//subset[i+1]의 맨 앞 child를 subset[i]의 맨 뒤로 옮김
			for (k = 1; subset[i + 1]->child_count; k++)
				subset[i + 1]->subset[k - 1] = subset[i + 1]->subset[k];
			subset[i + 1]->child_count--;
		}
	}
	else if (i != 0 && subset[i - 1]->data_count == MINIMUM)
	{
		subset[i - 1]->data[subset[i - 1]->data_count] = data[i - 1];									//this의 i-1의 데이터를 subset[i-1]로 옮김
		for (k = i; k < data_count; k++)												
			data[k-1] = data[k];
		data_count--;
		subset[i - 1]->data_count++;

		for (k = 0; k < subset[i]->data_count; k++)										//subset[i]을 subset[i-1]로 전부 옮김
		{
			subset[i - 1]->data[subset[i - 1]->data_count] = subset[i]->data[k];
			subset[i - 1]->data_count++;
		}
		for (k = 0; k < subset[i]->child_count; k++)
		{
			subset[i - 1]->subset[subset[i - 1]->child_count] = subset[i]->subset[k];
			subset[i - 1]->child_count++;
		}

		delete subset[i];																//subset[i] 없애고 뒤에 있는 subset들 앞으로 당김
		for (k = i + 1; k < child_count; k++)
			subset[k - 1] = subset[k];
		child_count--;
	}
	else if (i < child_count-1 && subset[i + 1]->data_count == MINIMUM)
	{
		subset[i]->data[subset[i]->data_count] = data[i];									//this의 i의 데이터를 subset[i]로 옮김
		for (k = i; k < data_count; k++)
			data[k] = data[k+1];
		data_count--;
		subset[i]->data_count++;

		for (k = 0; k < subset[i+1]->data_count; k++)										//subset[i+1]을 subset[i]로 전부 옮김
		{
			subset[i]->data[subset[i]->data_count] = subset[i+1]->data[k];
			subset[i]->data_count++;
		}
		for (k = 0; k < subset[i+1]->child_count; k++)
		{
			subset[i]->subset[subset[i]->child_count] = subset[i+1]->subset[k];
			subset[i]->child_count++;
		}
		delete subset[i + 1];
		for (k = i + 2; k < child_count; k++)												//delete subset[i+1] 하고 빈공간 채우기
			subset[k - 1] = subset[k];
		child_count--;
	}

	
 }
template <class Item>
void set<Item>::remove_biggest(Item& removed_entry)
{
	if (is_leaf())
	{
		removed_entry = data[data_count - 1];
		data_count--;
	}
	else
	{
		subset[child_count - 1]->remove_biggest(removed_entry);
		if (subset[child_count - 1]->data_count < MINIMUM) {
			fix_shortage(child_count - 1);
		}
	}
}

template <class Item>
std::size_t set<Item>::erase(const Item& target)
{
	if (!root_ptr->loose_erase(target))
		return false; 
	if ((root_ptr->data_count == 0) && (root_ptr->child_count == 1))
	{
		root_ptr = root_ptr->subset[0];
	}
	return true;
}