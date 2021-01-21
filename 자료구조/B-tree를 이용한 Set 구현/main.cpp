#include <iostream>
#include <string>
#include <cstdlib>
#include "set_tmp.h"
using namespace std;

int main()
{
	set<int> *bTree = new set<int>();
	int num;
	string s;
	while (1) {
		cout << "Input a command : ";
		cin >> s;
		if (!s.compare("insert"))
		{
			cout << "Input a number : ";
			cin >> num;
			bTree->insert(num);
			cout << endl << "============== print B-Tree ===============" << endl;
			bTree->show_content();
		}
		else if (!s.compare("erase"))
		{
			cout << "Input a number : ";
			cin >> num;
			bTree->erase(num);
			cout << endl << "============== print B-Tree ===============" << endl;
			bTree->show_content();
		}
		else if (!s.compare("count"))
		{
			cout << "Input a number : ";
			cin >> num;
			cout << bTree->count(num) << endl;
		}
		else if (!s.compare("stop"))
		{
			break;
		}
	}
	set<int> *copy_bTree1 = new set<int>(*bTree);
	set<int> *copy_bTree2 = new set<int>();
	copy_bTree2 = bTree;
	cout << endl << "============== print copy B-Tree 1 ===============" << endl;
	copy_bTree1->show_content();
	cout << endl << "============== print copy B-Tree 2 ===============" << endl;
	copy_bTree2->show_content();

	return 0;

}