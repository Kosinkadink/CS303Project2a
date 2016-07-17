#include "Evaluator.h"
#include <iostream>
#include "Infix_To_Postfix.h"
#include <string>
#include "Postfix_Evaluator.h"

using namespace std;

int main()
{

	std::string teststring = "1/0";


    Evaluator eval;
    eval.eval(teststring);

	return 0;
}