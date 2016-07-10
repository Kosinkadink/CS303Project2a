#pragma once


#include <string>
#include <sstream>
#include <stack>
#include <list>
#include "EvaluatorError.h"

using namespace std;

class Evaluator
{
public:
	Evaluator() {}
	int eval(string expression);
private:

};


int eval(string expression)
{
	stringstream firstparse(expression);
	char ch, ch2;

	list<string> coolPostfix;
	int intpart;
	string firstresult,postfixresult,part;
	while (firstparse >> ch)
	{
		if (isdigit(ch))
		{
			firstparse.putback(ch);
			firstparse >> intpart;
			coolPostfix.push_back()
		}
		else if (ch == '+')
		{
			if (firstparse >> ch2)
			{
				if (ch2)
			}
		}



	}



}