#include <stdlib.h>
#include <string.h>
#include <iostream>
#include "dnode.h"
#include "deque.h"
#include "deque.cpp"

using namespace std;

class evaluator     //계산기 클래스
{
private:
	double result;
public:
	double evaluate(string expr)  //postfix로 변환한 연산식 계산하는 함수
	{
		deque<double> eval;
		double value;								//연산식 길이 만큼 스택의 사이즈 설정해 스택 생성
		for (int i = 0; i < expr.length(); i++)    //문자열 하나씩 본다
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
			else if (expr[i] == '+')                //그 외 다른 연산자 기호라면 스택에서 두개를 pop해 연산해서 다시 스택에 넣어준다
			{
				double op2 = eval.back();   // char 형 임으로 '0'을 빼주어 정수로 만들어준다.
				eval.pop_back();
				double op1 = eval.back();
				eval.pop_back();
				eval.push_back(op1 + op2);
			}
			else if (expr[i] == '-')
			{
				double op2 = eval.back();   // char 형 임으로 '0'을 빼주어 정수로 만들어준다.
				eval.pop_back();
				double op1 = eval.back();
				eval.pop_back();
				eval.push_back(op1 - op2);
			}
			else if (expr[i] == '*')
			{
				double op2 = eval.back();   // char 형 임으로 '0'을 빼주어 정수로 만들어준다.
				eval.pop_back();
				double op1 = eval.back();
				eval.pop_back();
				eval.push_back(op1 * op2);
			}
			else if (expr[i] == '/')
			{
				double op2 = eval.back();   // char 형 임으로 '0'을 빼주어 정수로 만들어준다.
				eval.pop_back();
				double op1 = eval.back();
				eval.pop_back();
				eval.push_back(op1 / op2);
			}
		}
		result = eval.front(); //최종적으로 스택에 남아있는 것을 result에 저장
		eval.pop_front();
		return result;
	}

	string infixToPostfix(string expr)   //infix를 postfix로 변환하는 함수
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
	
	bool IsOperator(char c)   //연산자인지 확인
	{
		if (c == '+' || c == '-' || c == '*' || c == '/')
		{
			return true;
		}
		return false;
	}
	
	bool IsOperand(char c)   //피연산자인지 확인
	{
		if (isdigit(c) || isalpha(c) || c == '.')
			return true;

		return false;
	}

	
	int operate_first(char op1, char op2)   // 연산자 우선순위를 결정해준다.
	{
		int op1Weight = operator_number(op1);   //첫번 째 연산자의 우선순위 값 저장
		int op2Weight = operator_number(op2);  //두번 째 연산자의 우선 순위 값 저장
		return op1Weight >= op2Weight ? true : false;  //첫번 째 연산자의 우선순위가 두번 째 보다 크거나 같으면 true 반환
	}
	
	int operator_number(char op)  // 연산자 우선순위를 정하기위해서 순서를 정해준다.
	{
		int number;
		switch (op)
		{
		case '+':          //+와 - 이면 number 1로 초기화
		case '-':
			number = 1;
			break;
		case '*':            //*와 / 이면 number 1로 초기화
		case '/':
			number = 2;
		}
		return number;
	}
};