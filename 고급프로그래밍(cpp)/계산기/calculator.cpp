/*
	calculator_buggy.cpp
*/

#include "std_lib_facilities.h"

struct Token {
	char kind;
	double value;
	string name;
	Token(char ch) :kind(ch), value(0) { }
	Token(char ch, double val) :kind(ch), value(val) { }
	Token(char ch, string n) :kind(ch), name(n) { }
};

class Token_stream {
	bool full;
	Token buffer;
public:
	Token_stream() :full(0), buffer(0) { }

	Token get();
	void putback(Token t) { buffer=t; full=true; }
	void ignore(char);
};

const char let = 'L';
const char quit = 'Q';
const char print = ';';
const char number = '8';
const char name = 'a';
const char root = 'r';							//add case square root
const char func = 'f';							//add case trigonometrical functions

Token Token_stream::get()
{
	if (full) { full=false; return buffer; }  //If buffer has character, use buffer.
	char ch;
	cin >> ch;
	switch (ch) {
	case '(':
	case ')':
	case '+':
	case '-':
	case '*':
	case '/':
	case '%':				//add remainer operator
	case '!':				//add factorial operator
	case '^':				//(optiaonl)add Nsquare operator ex) 2^3 = 8
	case ';':
	case '=':
		return Token(ch);	//If you input these cases, Token kind is ch and value is empty.
	case '.':
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	{	
		cin.unget();					//putback
		double val;										
		cin >> val;						//get all numbers before operators
		return Token(number,val);
	}
	default:
		if (isalpha(ch)) {
			string s;
			s += ch;
			while(cin.get(ch) && (isalpha(ch) || isdigit(ch))) s += ch;
			cin.unget();
			if (s == "let") return Token(let);	
			if (s == "quit") return Token(quit);
			if (s == "sqrt") return Token(root);			//add square root
			if (s == "sin") return Token(func,s);			//add trigonometrical function 
			if (s == "cos") return Token(func,s);
			if (s == "tan") return Token(func,s);
			return Token(name,s);
		}
		error("Bad token");
	}
}

void Token_stream::ignore(char c)
{
	if (full && c==buffer.kind) {
		full = false;
		return;
	}
	full = false;

	char ch;
	while (cin>>ch)
		if (ch==c) return;
}

struct Variable {
	string name;
	double value;
	Variable(string n, double v) :name(n), value(v) { }
};

void mulTable99(int n) {
	
	for (int i = 1; i < 10; i++)
		cout << n << " * " << i << " = " << n * i << '\n';
}

vector<Variable> names;


double get_value(string s)
{
	names.push_back(Variable("pi", 3.14159));
	names.push_back(Variable("e", 2.71828));					//pre-declare pi, e
	for (int i = 0; i < names.size(); ++i)
		if (names[i].name == s) return names[i].value;
	error("get: undefined name ", s);
}

void set_value(string s, double d)
{
	for (int i = 0; i<=names.size(); ++i)
		if (names[i].name == s) {
			names[i].value = d;
			return;
		}
	error("set: undefined name ",s);
}

bool is_declared(string s)
{
	for (int i = 0; i<names.size(); ++i)
		if (names[i].name == s) return true;
	return false;
}

Token_stream ts;

double expression();

double primary()
{
	Token t = ts.get();
	switch (t.kind) {
	case 'r':													//case square root
	{
		double d;
		t = ts.get();
		if (t.kind == '(')
		{
			double exp = expression();							//calculate inside ( )
			if (exp < 0)										//when you get negative number
				error("Please input positive number.");
			d = sqrt(exp);										//get result
			t = ts.get();										//get ')'
			ts.putback(t);
			ts.ignore(t.kind);									//ignore ')'
		}
		else
			error("'(' expected");
		return d;
	}
	case 'f':													//case trigonometrical functions  
	{	
		if (t.name == "sin")
		{
			double d;
			t = ts.get();
			if (t.kind == '(')
			{
				double exp = expression();						//get inside ()
				d = sin(exp);									//get result 'sin'
				t = ts.get();									//get ')'
				ts.putback(t);
				ts.ignore(t.kind);								//ignore ')'
			}
			return d;
		}
		else if (t.name == "cos")
		{
			double d;
			t = ts.get();
			if (t.kind == '(')
			{
				double exp = expression();						//get inside ()
				d = cos(exp);									//get result 'cos'
				t = ts.get();									//get ')'
				ts.putback(t);				
				ts.ignore(t.kind);								//ignore ')'
			}
			return d;
		}
		else if (t.name == "tan")	
		{
			double d;
			t = ts.get();
			if (t.kind == '(')
			{
				double exp = expression();						//get inside ()
				d = tan(exp);									//get result 'tan'					
				t = ts.get();									//get ')'
				ts.putback(t);
				ts.ignore(t.kind);								//ignore ')'
			}
			return d;
		}
	}
	case '(':
	{	double d = expression();
		t = ts.get();
		if (t.kind != ')') error("')' expected");

		return d;					//There was not return(bug correction)
	}
	case '-':
		return -primary();
	case '+':
		return primary();			//unary plus
	case number:
		return t.value;
	case name:
		return get_value(t.name);
	default:
		error("primary expected");
	}
}

double pre_term()					//bind factorial tighter than any other operators
{
	double left = primary();
	while (true) {
		Token t = ts.get();
		switch (t.kind) {
		case '!':
		{
			int temp = int(left);
			int factorial = 1;
			for (int i = 1; i <= temp; i++)
				factorial *= i;
			return factorial;
		}
		case'^':									//(optional) multiply by its square
		{
			t = ts.get();
			double Nsquare = 1;
			for (int i = 1; i <= t.value; ++i)
				Nsquare *= left;
			return Nsquare;
		}
		default:
			ts.putback(t);
			return left;
		}
	}
		
}

double term()
{
	double left = pre_term();
	while(true) {
		Token t = ts.get();
		switch (t.kind) {
		case '*':
			left *= pre_term();
			break;
		case '/':
		{	
			double d = pre_term();
			if(d == 0) error("divide by zero");
			left /= d;
			break;
		}
		case '%':
			left = int(left) % int(pre_term());
			break;
		default:
			ts.putback(t);
			return left;
		}
		
	}
}

double expression()
{
	double left = term();
	while(true) {
		Token t = ts.get();
		switch(t.kind) {
		case '+':
			left += term();
			break;
		case '-':
			left -= term();
			break;
		default:
			ts.putback(t);
			return left;
		}
	}
}

double declaration()
{
	Token t = ts.get();
	if (t.kind != name) error ("name expected in declaration");
	string name = t.name;
	if (is_declared(name)) error(name, " declared twice");
	Token t2 = ts.get();
	if (t2.kind != '=') error("= missing in declaration of " ,name);
	double d = expression();
	if (name == "TimesTable")				//if you input "let TimesTable  = 5", get  5 times table
	{
		names.push_back(Variable("TimesTable", int(d)));
		mulTable99(d);
	}
	names.push_back(Variable(name, d));
	return d;
}

double statement()
{
	Token t = ts.get();
	switch(t.kind) {
	case let:
		return declaration();
	default:
		ts.putback(t);
		return expression();
	}
}

void clean_up_mess()
{
	ts.ignore(print);
}

const string prompt = "> ";
const string result = "= ";

void calculate()
{
	while(true) try {
		cout << prompt;
		Token t = ts.get();
		while (t.kind == print) t=ts.get();
		if (t.kind == quit) return;
		ts.putback(t);
		cout << result << statement() << endl;
	}
	catch(runtime_error& e) {
		cerr << e.what() << endl;
		clean_up_mess();
	}
}

int main()
try {
	calculate();
	return 0;
}
catch (exception& e) {
	cerr << "exception: " << e.what() << endl;
	return 1;
}
catch (...) {
	cerr << "exception\n";
	return 2;
}
