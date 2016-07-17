#include "Evaluator.h"
#include <iostream>
#include "Infix_To_Postfix.h"
#include <string>
#include "Postfix_Evaluator.h"

using namespace std;

int main()
{
    int output = 0;
	Infix_To_Postfix itp;
	Postfix_Evaluator pe;

	string teststring = "<0";

    teststring = itp.prepare(teststring);
     if (teststring != "")
    {
        string teststring2 = "+ 1";
        string newstring = itp.convert(teststring);

        output = pe.eval(newstring);
        cout << output << endl;
        system("pause");
    }

	return 0;
}