#pragma once

#include <string>
using namespace std;

class EvaluatorError : public exception
{
public:
    EvaluatorError(string e, char off = '.') { msg = e; offender = off; }
	string what() { return msg; }
private:
	string msg;
    char offender;
};