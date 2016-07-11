#pragma once

#include <string>
using namespace std;

class EvaluatorError : public exception
{
public:
	EvaluatorError(string e) { msg = e; }
	string what() { return msg; }
private:
	string msg;
};