#include "Evaluator.h"
#include "Infix_To_Postfix.h"
#include <string>
#include "Postfix_Evaluator.h"

int main()
{
	Infix_To_Postfix itp;
	Postfix_Evaluator pe;

	string teststring = "1 + 3 > 2";
	string teststring2 = "~ 1";
	string newstring = itp.convert(teststring2);

	int output = pe.eval(newstring);

	return 0;
}