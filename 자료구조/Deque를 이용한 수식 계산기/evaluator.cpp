#include <stdlib.h>
#include <string.h>
#include <iostream>
#include "dnode.h"
#include "deque.h"
#include "deque.cpp"

using namespace std;

class evaluator     //���� Ŭ����
{
private:
	double result;
public:
	double evaluate(string expr)  //postfix�� ��ȯ�� ����� ����ϴ� �Լ�
	{
		deque<double> eval;
		double value;								//����� ���� ��ŭ ������ ������ ������ ���� ����
		for (int i = 0; i < expr.length(); i++)    //���ڿ� �ϳ��� ����
		{
			if (IsOperand(expr[i]))
			{
				if (IsOperand(expr[i + 1]))
				{
					char tmp[20];

					int j = 0;
					while (IsOperand(expr[i]))
					{
						tmp[j] = expr[i];
						i++;
						j++;
					}

					tmp[j] = '\0';
					--i;
					double tmp2 =(double)atoi(tmp);
					eval.push_back(tmp2);
				}
				else if (isalpha(expr[i]))
				{
					cout << "> Input " << expr[i] << " value : ";
					cin >> value;
					eval.push_back(value);
				}
				else
				{
					eval.push_back(expr[i] - '0');
				}
			}
			else if (expr[i] == '+')                //�� �� �ٸ� ������ ��ȣ��� ���ÿ��� �ΰ��� pop�� �����ؼ� �ٽ� ���ÿ� �־��ش�
			{
				double op2 = eval.back();   // char �� ������ '0'�� ���־� ������ ������ش�.
				eval.pop_back();
				double op1 = eval.back();
				eval.pop_back();
				eval.push_back(op1 + op2);
			}
			else if (expr[i] == '-')
			{
				double op2 = eval.back();   // char �� ������ '0'�� ���־� ������ ������ش�.
				eval.pop_back();
				double op1 = eval.back();
				eval.pop_back();
				eval.push_back(op1 - op2);
			}
			else if (expr[i] == '*')
			{
				double op2 = eval.back();   // char �� ������ '0'�� ���־� ������ ������ش�.
				eval.pop_back();
				double op1 = eval.back();
				eval.pop_back();
				eval.push_back(op1 * op2);
			}
			else if (expr[i] == '/')
			{
				double op2 = eval.back();   // char �� ������ '0'�� ���־� ������ ������ش�.
				eval.pop_back();
				double op1 = eval.back();
				eval.pop_back();
				eval.push_back(op1 / op2);
			}
		}
		result = eval.front(); //���������� ���ÿ� �����ִ� ���� result�� ����
		eval.pop_front();
		return result;
	}

	string infixToPostfix(string expr)   //infix�� postfix�� ��ȯ�ϴ� �Լ�
	{
		deque<char> op;
		string postfix;
		size_t count = expr.length();

		for (int i = 0; i < count; i++)
		{
			if (expr[i] == '(')
			{
				op.push_front(expr[i]);
			}
			else if (IsOperand(expr[i]))
			{
				while (IsOperand(expr[i]))
				{
					postfix += expr[i];
					i++;
				}
				--i;
				postfix += " ";
			}
			else if (IsOperator(expr[i]))
			{
				while (!op.empty() && op.front() != '(' && operate_first(op.front(), expr[i]))
				{
					postfix += op.front();
					op.pop_front();
				}
				op.push_front(expr[i]);
			}
			else if (expr[i] == ')')
			{
				while (!op.empty() && op.front() != '(')
				{
					postfix += op.front();
					op.pop_front();
				}
				op.pop_front();
			}
		}

		while (!op.empty())
		{
			postfix += op.front();
			op.pop_front();
		}

		return postfix;
	}
	
	bool IsOperator(char c)   //���������� Ȯ��
	{
		if (c == '+' || c == '-' || c == '*' || c == '/')
		{
			return true;
		}
		return false;
	}
	
	bool IsOperand(char c)   //�ǿ��������� Ȯ��
	{
		if (isdigit(c) || isalpha(c) || c == '.')
			return true;

		return false;
	}

	
	int operate_first(char op1, char op2)   // ������ �켱������ �������ش�.
	{
		int op1Weight = operator_number(op1);   //ù�� ° �������� �켱���� �� ����
		int op2Weight = operator_number(op2);  //�ι� ° �������� �켱 ���� �� ����
		return op1Weight >= op2Weight ? true : false;  //ù�� ° �������� �켱������ �ι� ° ���� ũ�ų� ������ true ��ȯ
	}
	
	int operator_number(char op)  // ������ �켱������ ���ϱ����ؼ� ������ �����ش�.
	{
		int number;
		switch (op)
		{
		case '+':          //+�� - �̸� number 1�� �ʱ�ȭ
		case '-':
			number = 1;
			break;
		case '*':            //*�� / �̸� number 1�� �ʱ�ȭ
		case '/':
			number = 2;
		}
		return number;
	}
};