/** Implementation of Infix_To_Postfix that processes parentheses.*/
#include <iostream>
#include "EvaluatorError.h"
#include "Infix_To_Postfix.h"
#include <sstream>
#include <cctype>
using std::string;
using std::istringstream;

const string Infix_To_Postfix::OPERATORS = "!t_~^*/%+->g<len&|()[]{}";
const int Infix_To_Postfix::PRECEDENCE[] = { 8, 8, 8, 8, 7, 6, 6, 6, 5, 5, 4, 4, 4, 4, 3, 3, 2, 1, -1, -1, -1, -1, -1, -1 };

/** Extract and process each token in infix and return the
    equivalent postfix string.
    @param expression The infix expression
    @return The equivalent postfix expression
    @throws Syntax_Error
*/


std::string Infix_To_Postfix::prepare(const std::string& expression)
{
    std::stack<char> parenth_stack;
    char before = NULL, current = NULL, next = NULL, temp;
    std::string prepared = "";
    string str_nums;
    int nums;
    std::stringstream parse(expression);
    try{
        while (parse >> current)
        {
            if (current == '-')
            {
                parse >> next;
                // first character in string is a -
                if (before == NULL)
                {
                    // negative number
                    if (isdigit(next))
                    {
                        parse.putback(next);
                        prepared.push_back('~');
                        prepared.push_back(' ');
                        before = '~';
                    }
                    // decrement
                    else if (next == '-')
                    {
                        prepared.push_back('_');
                        prepared.push_back(' ');
                        before = '_';
                    }
                }
                // negative after a parenthesis or power
                else if (before == '(' || before == '{' || before == '[' || before == '^')
                {
                    // negative number
                    if (isdigit(next))
                    {
                        parse.putback(next);
                        prepared.push_back('~');
                        prepared.push_back(' ');
                        before = '~';
                    }
                    // decrement
                    else if (next == '-')
                    {
                        prepared.push_back('_');
                        prepared.push_back(' ');
                        before = '_';
                    }
                }
                // decrement
                else if (next == '-')
                {
                    prepared.push_back('_');
                    prepared.push_back(' ');
                    before = '_';
                }
                // subtraction
                else if (isdigit(next) && isdigit(before))
                {
                    parse.putback(next);
                    prepared.push_back('-');
                    prepared.push_back(' ');
                    before = '-';
                }
				else if (next == '(' || next == '{' || next == '[')
				{
					parse >> temp;
					// addition
					if (isdigit(temp) && isdigit(before))
					{
						parse.putback(temp);
						parse.putback(next);
						prepared.push_back('-');
						prepared.push_back(' ');
						before = '-';
					}
				}
            }
            else if (current == '+')
            {
                parse >> next;
                // first character of the string
                if (before == NULL)
                {
                    // posetive number
                    if (isdigit(next))
                    {
                        parse.putback(next);
                    }
                    // increment
                    else if (next == '+')
                    {
                        prepared.push_back('t');
                        prepared.push_back(' ');
                        before = 't';
                    }
                }
                // + sign after a parenthesis or power
                else if (before == '(' || before == '{' || before == '[' || before == '^')
                {
                    // posetive number
                    if (isdigit(next))
                    {
                        parse.putback(next);
                    }
                    // increment
                    else if (next == '+')
                    {
                        prepared.push_back('t');
                        prepared.push_back(' ');
                        before = 't';
                    }
                }
                // increment
                else if (next == '+')
                {
                    prepared.push_back('t');
                    prepared.push_back(' ');
                    before = 't';
                }
                // addition
                else if (isdigit(next) && isdigit(before))
                {
                    parse.putback(next);
                    prepared.push_back('+');
                    prepared.push_back(' ');
                    before = '+';
                }
				else if (next == '(' || next == '{' || next == '[')
				{
					parse >> temp;
					// addition
					if (isdigit(temp) && isdigit(before))
					{
						parse.putback(temp);
						parse.putback(next);
						prepared.push_back('+');
						prepared.push_back(' ');
						before = '+';
					}
				}
            }
            // multiply, divide, or mod; check that they are bracketed by numbers
            else if (current == '/' || current == '*' || current == '%')
            {
                parse >> next;
                // legal
                if (before == ')' || before == '}' || before == ']' || (isdigit(before)) && (isdigit(next) || next == '(' || next == '{' || next == '['))
                {
                    parse.putback(next);
                    prepared.push_back(current);
                    prepared.push_back(' ');
                }
                else
                    throw EvaluatorError("must be bracketed by numbers.");
            }
            // next several values may be one number, catch them all
            else if (isdigit(current))
            {
                if (!isdigit(before))
                {
                    before = current;
                    parse.putback(current);
                    parse >> nums;
                    str_nums = to_string(nums);
                    prepared += (str_nums);
                    prepared += ' ';
                }
                else
                    throw EvaluatorError("cannot have two consecutive numbers.");
            }
            // determine if a not operator of not equal to
            else if (current == '!')
            {
                parse >> next;
                // not equal to
                if (next == '=')
                {
                    parse >> next;
                    // make sure a digit follows the logical operator
                    if (isdigit(next))
                    {
                        parse.putback(next);
                        prepared += 'n';
                        prepared += ' ';
                        before = 'n';
                    }
                    else
                        throw EvaluatorError("an integer must follow a logical operator.");
                }
                // not operator
                else if (isdigit(next) || next == '(' || next == '{' || next == '[')
                {
                    parse.putback(next);
                    prepared += '!';
                    prepared += ' ';
                    before = '!';
                }
                else
                    throw EvaluatorError("! needs to be attached to an integer.");
            }
            // equivalency 
            else if (current == '=')
            {
                parse >> next;
                // legal
                if (next == '=')
                {
                    parse >> next;
                    // still legal
                    if (isdigit(next) || next == '(' || next == '{' || next == '[')
                    {
                        parse.putback(next);
                        prepared += '=';
                        prepared += ' ';
                        before = '=';
                    }
                }
            }
            // power
            else if (current == '^')
            {
                // valid power
                if (before == ')' || before == '}' || before == ']' || isdigit(before))
                {
                    parse >> next;
                    // valid data following
                    if (isdigit(next))
                    {
                        parse.putback(next);
                        prepared += '^';
                        prepared += ' ';
                        before = '^';
                    }
                    // also valid
                    else if (next == '(' || next == '{' || next == '[' || next == '-')
                    {
                        // still looking for a number
                        parse >> temp;
                        if (isdigit(temp))
                        {
                            parse.putback(temp);
                            parse.putback(next);
                            prepared += '^';
                            prepared += ' ';
                            before = '^';
                        }
                        else
                            throw EvaluatorError("A power needs a following number.");
                    }
                    else
                        throw EvaluatorError("A power needs a following number.");
                }
                else
                    throw EvaluatorError("A power must follow a parenthesis or number.");
            }
            // < or <=
            else if (current == '<')
            {
                parse >> next;
                // look for a digit
                if (isdigit(next))
                {
                    parse.putback(next);
                    prepared += '<';
                    prepared += ' ';
                    before = '<';
                }
                // keep looking
                else if (next == '(' || next == '{' || next == '[' || next == '-')
                {
                    parse >> temp;
                    if (isdigit(temp))
                    {
                        parse.putback(temp);
                        parse.putback(next);
                        prepared += '<';
                        prepared += ' ';
                        before = '<';
                    }
                }
                // <=
                else if (next == '=')
                {
                    parse >> next;
                    // look for a digit
                    if (isdigit(next))
                    {
                        parse.putback(next);
                        prepared += 'l';
                        prepared += ' ';
                        before = 'l';
                    }
                    // keep looking
                    else if (next == '(' || next == '{' || next == '[' || next == '-')
                    {
                        parse >> temp;
                        if (isdigit(temp))
                        {
                            parse.putback(temp);
                            parse.putback(next);
                            prepared += 'l';
                            prepared += ' ';
                            before = 'l';
                        }
                    }
                }
            }
            else if (next == '>')
            {
                parse >> next;
                // look for a digit
                if (isdigit(next))
                {
                    parse.putback(next);
                    prepared += '>';
                    prepared += ' ';
                    before = '>';
                }
                // keep looking
                else if (next == '(' || next == '{' || next == '[' || next == '-')
                {
                    parse >> temp;
                    if (isdigit(temp))
                    {
                        parse.putback(temp);
                        parse.putback(next);
                        prepared += '>';
                        prepared += ' ';
                        before = '>';
                    }
                }
                // <=
                else if (next == '=')
                {
                    parse >> next;
                    // look for a digit
                    if (isdigit(next))
                    {
                        parse.putback(next);
                        prepared += 'g';
                        prepared += ' ';
                        before = 'g';
                    }
                    // keep looking
                    else if (next == '(' || next == '{' || next == '[' || next == '-')
                    {
                        parse >> temp;
                        if (isdigit(temp))
                        {
                            parse.putback(temp);
                            parse.putback(next);
                            prepared += 'g';
                            prepared += ' ';
                            before = 'g';
                        }
                    }
                }
            }
            // logical and
            else if (current == '&')
            {
                parse >> next;
                if (next == '&' && (before == ')' || before == '}' || before == ']' || isdigit(before)))
                {
                    parse >> next;
                    if (next == '(' || next == '{' || next == '[')
                    {
                        parse >> temp;
                        if (isdigit(temp))
                        {
                            parse.putback(temp);
                            parse.putback(next);
                            prepared += '&';
                            prepared += ' ';
                            before = '&';
                        }
                    }
                    else if (isdigit(next))
                    {
                        parse.putback(next);
                        prepared += '&';
                        prepared += ' ';
                        before = '&';
                    }
                }
            }
            // logical or
            else if (current == '|')
            {
                parse >> next;
                if (next == '|' && (before == ')' || before == '}' || before == ']' || isdigit(before)))
                {
                    parse >> next;
                    if (next == '(' || next == '{' || next == '[')
                    {
                        parse >> temp;
                        if (isdigit(temp))
                        {
                            parse.putback(temp);
                            parse.putback(next);
                            prepared += '|';
                            prepared += ' ';
                            before = '|';
                        }
                    }
                    else if (isdigit(next))
                    {
                        parse.putback(next);
                        prepared += '|';
                        prepared += ' ';
                        before = '|';
                    }
                }
            }
            // parenthesis
            else if (current == '(' || current == '{' || current == '[')
            {
                prepared += current;
                prepared += ' ';
                parenth_stack.push(current);
                before = current;
            }
            // closing parenthesis
            else if ((current == ')' || current == '}' || current == ']'))
            {
                if (current == ')' && !parenth_stack.empty())
                {
                    temp = parenth_stack.top();
                    if (temp != '(' )
                        throw EvaluatorError("incorrect parenthesis usage.");
                }
				else if (current == '}' && !parenth_stack.empty())
                {
                    temp = parenth_stack.top();
                    if (temp != '{')
                        throw EvaluatorError("incorrect parenthesis usage.");
                }
				else if (current == ']' && !parenth_stack.empty())
				{
					temp = parenth_stack.top();
					if (temp != '[')
						throw EvaluatorError("incorrect parenthesis usage.");
				}
				else
					throw EvaluatorError("missing opening parenthesis.");
                prepared += current;
                prepared += ' ';
                before = ')';
                parenth_stack.pop();
            }
        }
        if (!parenth_stack.empty())
        {
            throw EvaluatorError("missing a closing parenthesis.");
        }
    }
    catch (EvaluatorError error)
    {
        cout << error.what() << endl;
        system("pause");
        prepared = "";
    }
    return prepared;
}


string Infix_To_Postfix::convert(const string& expression){
  postfix = "";
  while (!operator_stack.empty()) 
    operator_stack.pop();
  istringstream infix_tokens(expression);
  string next_token;
  while(infix_tokens >> next_token) {
    if (isdigit(next_token[0])) {
      postfix += next_token;
      postfix += " ";
    } else if (is_operator(next_token[0])) {
      process_operator(next_token[0]);
    } else {
      throw Syntax_Error("Unexpected Character Encountered");
    }
  } // End while
  // Pop any remaining operators and append them to postfix
  while (!operator_stack.empty()) {
    char op = operator_stack.top();
    operator_stack.pop();
    if (op == '(' || op == '[' || op == '}') {
      throw Syntax_Error("Unmatched open parenthesis");
    }
    postfix += op;
    postfix += " ";
  }
  return postfix;
}

/** Function to process operators.
    @param op The operator
    @throws Syntax_Error
*/
void Infix_To_Postfix::process_operator(char op) {
  if (operator_stack.empty() || (op == '(') || (op == '[') || (op == '{')) {
    if (op == ')' || op == ']' || op == '}')
      throw Syntax_Error("Unmatched close parenthesis");
    operator_stack.push(op);
  } else {
    if (precedence(op) > precedence(operator_stack.top()) || op=='t' || op=='!' || op=='_') {
      operator_stack.push(op);
    } else {
      // Pop all stacked operators with equal
      // or higher precedence than op.
      while (!operator_stack.empty()
             && (operator_stack.top() != '(')
	     && (operator_stack.top() != '[')
	     && (operator_stack.top() != '{')
             && (precedence(op) <= precedence(operator_stack.top()))) {
        postfix += operator_stack.top();
        postfix += " ";
        operator_stack.pop();
      }
      // assert: Operator stack is empty or 
      //         top of stack is '(' or current
      //         operator precedence > top of stack operator
      //         precedence;
      if (op == ')') {
        if (!operator_stack.empty() 
            && (operator_stack.top() == '(')) {
          operator_stack.pop();
        } else {
          throw Syntax_Error("Unmatched close parentheses");
        }
      } else if (op == ']') {
        if (!operator_stack.empty() 
            && (operator_stack.top() == '[')) {
          operator_stack.pop();
        } else {
          throw Syntax_Error("Unmatched close parentheses");
        }
      } else if (op == '}') {
        if (!operator_stack.empty() 
            && (operator_stack.top() == '{')) {
          operator_stack.pop();
        } else {
          throw Syntax_Error("Unmatched close parentheses");
        }
      } else {
        operator_stack.push(op);
      }
    }
  }
}
