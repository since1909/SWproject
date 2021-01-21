#include "bag.h"
#include <iostream>
using namespace main_savitch_5;
using namespace std;

int main()
{
	bag b1, b2, b3, b4, b5, b6, b8;																					
	b1.insert(5);
	b1.insert(4);
	b1.insert(3);
	b1.insert(2);
	b1.insert(1);
	
	cout << "Bag b1:\n";
	b1.show_content();
	
	cout << '\n' << "Reverse the bag b1:\n";
	b1.reverse();
	b1.show_content();
	
	cout << '\n' << "erase '3' in the bag b1:\n";
	b1.erase_one(3);
	b1.show_content();

	b2.insert(5);
	b2.insert(5);
	b2.insert(5);
	b2.insert(1);
	b2.insert(1);
	
	cout << '\n' << "Bag b2:\n";
	b2.show_content();

	cout << '\n' << "count '1' in the bag b2:  ";
	cout << b2.count(1) << '\n';
	cout << "count '5' in the bag b2:  ";
	cout << b2.count(5) << '\n';

	cout << '\n' << "Remove repetitions in the bag b2:\n";
	b2.removeRepetition();
	b2.show_content();

	
	b3.insert(6);
	b3.insert(5);
	b3.insert(4);

	b4.insert(3);
	b4.insert(2);
	b4.insert(1);

	cout << '\n' <<"Bag b3:\n";
	b3.show_content();
	cout << '\n' << "Bag b4:\n";
	b4.show_content();
	cout << '\n' << "Bag b5 = b3 + b4:\n";
	b5 = b3 + b4;
	b5.show_content();
	cout << '\n' << "Bag b5 += b4:\n";
	b5 += b4;
	b5.show_content();

	b6.insert(6);
	b6.insert(5);
	b6.insert(4);
	b6.insert(3);
	b6.insert(2);
	b6.insert(1);
	
	cout << '\n' << "Bag b6:\n";
	b6.show_content();
	cout << '\n' << "Bag b6 - b3:\n";
	b6 = b6 - b3;
	b6.show_content();
	cout << '\n' << "Bag b6 - b4:\n";
	b6 -= b4;
	b6.show_content();

	cout << '\n' << "Bag b6 = b4:\n";
	b6 = b4;
	b6.show_content();

	bool result = (b3 == b4);
	cout << '\n' << "b3 == b4 ?:  ";
	cout << result << '\n';

	bag b7(b6);
	cout << '\n' << "Bag b7(= b6): \n";
	b7.show_content();

	cout << '\n' << "Bag b8:\n";
	b8.insert(2);
	b8.insert(10);
	b8.insert(4);
	b8.insert(8);
	b8.insert(6);
	b8.show_content();
	cout << '\n' << "Sort the bag b8:\n";
	b8.sort();
	b8.show_content();

}
