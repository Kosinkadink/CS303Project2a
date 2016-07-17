#pragma once

#include <iostream>
#include "Infix_To_Postfix.h"
#include "Postfix_Evaluator.h"
#include <string>
#include <sstream>
#include <stack>
#include <list>
#include "EvaluatorError.h"

using namespace std;

class Evaluator
{
public:
    Evaluator() { ; }
	void eval(string expression);
private:

};


void Evaluator::eval(string expression)
{
	
    int output = 0;
    Infix_To_Postfix itp;
    Postfix_Evaluator pe;

    string teststring;
    teststring = itp.prepare(expression);
    try{
        if (teststring != "")
        {
            string newstring = itp.convert(teststring);

            output = pe.eval(newstring);
            cout << output << endl;
            system("pause");
        }
    }
    catch (EvaluatorError e)
    {
        cout << "Your Infix expression was not valid, please enter a valid expression and try again." << endl;
        system("pause");
    }

}