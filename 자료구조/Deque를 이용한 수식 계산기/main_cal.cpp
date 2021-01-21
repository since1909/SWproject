#include "evaluator.cpp"
#include <iostream>
#include <string>
#include <sys/types.h>
#include <stdlib.h>
using namespace std;

int main()
{
	deque<int> D1;

	D1.push_front(2);
	D1.push_back(3);
	D1.push_front(1);
	D1.push_back(4);

	deque<int> D2(D1);
	deque<int> D3 = D1;

	for (int i = 0; i<4; i++)
	{
		cout << D1.front() << '\t';
		D1.pop_front();
	}

	cout << endl;

	while (!D2.empty())
	{
		cout << D2.back() << '\t';
		D2.pop_back();
	}
	cout << endl;

	
	for (int i = 0; i<4; i++)
	{
		cout << D3.front() << '\t';
		D3.pop_front();
	}

	cout << endl;

	evaluator C;
	string expression;
	cout << ">>> Input an vaild infix expression :	";
	getline(cin, expression);
	string postfix = C.infixToPostfix(expression);
	cout << ">>> Convert to postfix : " << postfix << endl;
	cout << ">>> The answer is : " << C.evaluate(postfix) << endl;

}