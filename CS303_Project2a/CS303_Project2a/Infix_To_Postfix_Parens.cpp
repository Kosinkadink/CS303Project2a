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

void Infix_To_Postfix::unstack(stringstream& parse)
{
	while (!temp_stack.empty())
	{
		parse.putback(temp_stack.top());
		temp_stack.pop();
	}
}


std::string Infix_To_Postfix::prepare(const std::string& expression)
{
    std::stack<char> parenth_stack;
	
    char before = NULL, current = NULL, next = NULL, temp;
    std::string prepared = "";
    string str_nums;
    int nums;
	int charnum = -1;
	int g = 0;
    std::stringstream parse(expression);
    try{
        while (parse >> current)
        {
			charnum++;

			//if first expression is a binary operator or closing parenthesis, throw error message
			while (g == 0) 
			{ 
				g = 1;
				if (current == '<' || current == '>' || current == '&' || current == '|') 
				{
					throw EvaluatorError("Expression cannot start with a binary operator.");
				}
				if (current == ')' || current == '}' || current == ']')
				{
					throw EvaluatorError("Expression cannot start with a closing parenthesis.");
				}

			}

            if (current == '-')
            {
                parse >> next;

                // first character in string is a -
				if (before == NULL || before == '(' || before == '{' || before == '[' || before == '^' || before == '&' || before =='|')
                {
                    // negative number
                    if (isdigit(next))
                    {
                        parse.putback(next);
                        prepared.push_back('~');
                        prepared.push_back(' ');
                        before = '~';
                    }
                    // be certain a digit exists beyond the parenthesis
					else if (next == '(' || next == '{' || next == '[')
					{
						do{
							temp_stack.push(next);
							parse >> next;
						} while (!isdigit(next));
						parse.putback(next);
						unstack(parse);
						prepared.push_back('~');
						prepared.push_back(' ');
						before = '~';

					}
                    // decrement
                    else if (next == '-')
                    {
						temp_stack.push(next);
						parse >> next;
						//third -
						if (next == '-')
						{
							temp_stack.push(next);
							parse >> next;
							// fourth -
							if (next == '-')
							{
								unstack(parse);
							}
							else{
								parse.putback(next);
								unstack(parse);
								prepared += '~';
								prepared += ' ';
								before += '~';
								continue;
							}
						}
						
                        prepared.push_back('_');
                        prepared.push_back(' ');
                        before = '_';
                        charnum++;
                    }
                }
                // verify a digit exists byond the parenthesis
			    else if (next == '(' || next == '{' || next == '[')
			    {
			    	do{
				    	temp_stack.push(next);
				    	parse >> next;
				    } while (!isdigit(next));
				    parse.putback(next);
				    unstack(parse);
				    prepared.push_back('-');
				    prepared.push_back(' ');
				    before = '-';		
		    	}
                // decrement
                else if (next == '-')
                {
                    parse >> next;
                    if (next == '-')
                    {
                        parse.putback(next);
                        parse.putback(next);
                        before = '-';
                        prepared += '-';
                        prepared += ' ';
                        continue;
                    }
                    parse.putback(next);
                    prepared.push_back('_');
                    prepared.push_back(' ');
                    before = '_';
                    charnum++;
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
                else if (isdigit(next) || next == '+')
                {
                    parse.putback(next);
                    prepared.push_back('~');
                    prepared.push_back(' ');
                    before = '~';
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
                        parse >> next;
                        prepared.push_back('t');
                        prepared.push_back(' ');
                        before = 't';
                        charnum++;
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
                        charnum++;
                    }
                }
                // increment
                else if (next == '+')
                {
                    parse >> next;
                    if (next == '+')
                    {
                        if (before == '&' || before == '|' || before == '<' || before == 'l' || before == '>' || before == 'g' || before == 'n' || before == 'e')
                        {
                            throw EvaluatorError("Addition must be bracketed by digits on both sides.");
                        }
                        parse.putback(next);
                        parse.putback(next);
                        prepared += '+';
                        prepared += ' ';
                        before = '+';
                        continue;
                    }
                    parse.putback(next);
                    prepared.push_back('t');
                    prepared.push_back(' ');
                    before = 't';
                    charnum++;
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
                    do{
                        temp_stack.push(next);
                        parse >> next;
                    } while (!isdigit(next));
                    parse.putback(next);
                    unstack(parse);
                    prepared.push_back('+');
                    prepared.push_back(' ');
                    before = '+';

				}
            }
            // multiply, divide, or mod; check that they are bracketed by numbers
            else if (current == '/' || current == '*' || current == '%')
            {
                parse >> next;
                // legal
                if (before == ')' || before == '}' || before == ']' || (isdigit(before)) && (isdigit(next) || next == '(' || next == '{' || next == '['))
                {
                    before = current;
                    parse.putback(next);
                    prepared.push_back(current);
                    prepared.push_back(' ');
                }
                else
                    throw EvaluatorError("Arithmatic operators must be bracketed by numbers.");
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
                    if (before == 't' || before == '_' || before == '!' || before == '~')
                        throw EvaluatorError("A binary operator cannot follow a unary operator.");
                    do{
                        parse >> next;

						if (next == '=' || next == '&' || next == '|' || next == '<' || next == '>')
						{
							throw EvaluatorError("Cannot have two binary operators in a row");
						}

                        temp_stack.push(next);
                    } while (!isdigit(next));
                    unstack(parse);
                    prepared.push_back('n');
                    prepared.push_back(' ');
                    before = 'n';
                    charnum++;

                }
                // not operator
                else if (isdigit(next) || next == '(' || next == '{' || next == '[')
                {
                    do{
                        temp_stack.push(next);
                        parse >> next;
                    } while (!isdigit(next));
                    parse.putback(next);
                    unstack(parse);
                    prepared.push_back('!');
                    prepared.push_back(' ');
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
                    if (before == 't' || before == '_' || before == '!' || before == '~' )
                        throw EvaluatorError("A binary operator cannot follow a unary operator.");
                    do{
                        parse >> next;

						if (next == '='||next=='&'||next=='|'||next=='<'||next=='>') 
						{ throw EvaluatorError("Cannot have two binary operators in a row"); }

                        temp_stack.push(next);
                    } while (!isdigit(next));
                    parse.putback(next);
                    unstack(parse);
                    prepared.push_back('e');
                    prepared.push_back(' ');
                    before = 'e';
                    charnum++;
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
						do {
							temp_stack.push(next);
							parse >> next;
						} while (!isdigit(next));
						parse.putback(next);
						unstack(parse);
						before = '^';
						prepared += '^';
						prepared += ' ';
						
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
                if (before == 't' || before == '_' || before == '!' || before == '~')
                    throw EvaluatorError("A binary operator cannot follow a unary operator.");
                parse >> next;
				if (next == '&' || next == '|' || next == '<' || next == '>')
				{
					throw EvaluatorError("Cannot have two binary operators in a row");
				}
                // look for a digit
                if (isdigit(next))
                {
                    parse.putback(next);
                    prepared += '<';
                    prepared += ' ';
                    before = '<';
                }
                // keep looking
                else if (next == '(' || next == '{' || next == '[' || next == '-' || next == '+')
                {
                    do{
                        temp_stack.push(next);
                        parse >> next;
                    } while (!isdigit(next));
                    parse.putback(next);
                    unstack(parse);
                    prepared.push_back('<');
                    prepared.push_back(' ');
                    before = '<';
                }
                // <=
                else if (next == '=')
                {
                    parse >> next;

					if (next == '=' || next == '&' || next == '|' || next == '<' || next == '>')
					{
						throw EvaluatorError("Cannot have two binary operators in a row");
					}
                    // look for a digit
                    if (isdigit(next))
                    {
                        parse.putback(next);
                        prepared += 'l';
                        prepared += ' ';
                        before = 'l';
                        charnum++;
                    }
                    // keep looking
                    else if (next == '(' || next == '{' || next == '[' || next == '-' || next == '+')
                    {
                        do{
                            temp_stack.push(next);
                            parse >> next;
                        } while (!isdigit(next));
                        parse.putback(next);
                        unstack(parse);
                        prepared.push_back('l');
                        prepared.push_back(' ');
                        before = 'l';
                        charnum++;
                    }
                }
            }
            else if (current == '>')
            {
                if (before == 't' || before == '_' || before == '!' || before == '~')
                    throw EvaluatorError("A binary operator cannot follow a unary operator.");
                parse >> next;
				if (next == '&' || next == '|' || next == '<' || next == '>')
				{
					throw EvaluatorError("Cannot have two binary operators in a row.");
				}
                // look for a digit
                if (isdigit(next))
                {
                    parse.putback(next);
                    prepared += '>';
                    prepared += ' ';
                    before = '>';
                }
                // keep looking
                else if (next == '(' || next == '{' || next == '[' || next == '-' || next == '+')
                {
                    do{
                        temp_stack.push(next);
                        parse >> next;
                    } while (!isdigit(next));
                    parse.putback(next);
                    unstack(parse);
                    prepared.push_back('>');
                    prepared.push_back(' ');
                    before = '>';
                }
                // <=
                else if (next == '=')
                {
                    parse >> next;

					if (next == '=' || next == '&' || next == '|' || next == '<' || next == '>')
					{
						throw EvaluatorError("Cannot have two binary operators in a row.");
					}
                    // look for a digit
                    if (isdigit(next))
                    {
                        parse.putback(next);
                        prepared += 'g';
                        prepared += ' ';
                        before = 'g';
                        charnum++;
                    }
                    // keep looking
                    else if (next == '(' || next == '{' || next == '[' || next == '-' || next == '+')
                    {
                        do{
                            temp_stack.push(next);
                            parse >> next;
                        } while (!isdigit(next));
                        parse.putback(next);
                        unstack(parse);
                        prepared.push_back('g');
                        prepared.push_back(' ');
                        before = 'g';
                        charnum++;
                    }
                }
            }
            // logical and
            else if (current == '&')
            {
                if (before == 't' || before == '_' || before == '!' || before == '~')
                    throw EvaluatorError("A logical operator cannot follow a unary operator.");
                parse >> next;
                if (next == '&' && (before == ')' || before == '}' || before == ']' || isdigit(before)))
                {
                    parse >> next;

					if (next == '='||next=='&'||next=='|'||next=='<'||next=='>') 
						{ throw EvaluatorError("Cannot have two binary operators in a row."); }

                    if (next == '(' || next == '{' || next == '[' || next == '-' || next == '+')
                    {
						do {
							temp_stack.push(next);
							parse >> next;
						} while (!isdigit(next));
						parse.putback(next);
						unstack(parse);
                        prepared += '&';
                        prepared += ' ';
                        before = '&';
                        charnum++;
                    }
                    else if (isdigit(next))
                    {
                        parse.putback(next);
                        prepared += '&';
                        prepared += ' ';
                        before = '&';
                        charnum++;
                    }
                }
            }
            // logical or
            else if (current == '|')
            {
                if (before == 't' || before == '_' || before == '!' || before == '~')
                    throw EvaluatorError("A logical operator cannot follow a unary operator.");
                parse >> next;
                if (next == '|' && (before == ')' || before == '}' || before == ']' || isdigit(before)))
                {
                    parse >> next;

					if (next == '=' || next == '&' || next == '|' || next == '<' || next == '>')
					{
						throw EvaluatorError("Cannot have two binary operators in a row.");
					}

                    if (next == '(' || next == '{' || next == '[')
                    {
                        do {
                            temp_stack.push(next);
                            parse >> next;
                        } while (!isdigit(next));
                        parse.putback(next);
                        unstack(parse);
                        prepared += '|';
                        prepared += ' ';
                        before = '|';
                        charnum++;
                    }
                    else if (isdigit(next))
                    {
                        parse.putback(next);
                        prepared += '|';
                        prepared += ' ';
                        before = '|';
                        charnum++;
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
        cout << error.what() << "  Error occured at character " << charnum << endl;
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
      throw EvaluatorError("Unexpected Character Encountered");
    }
  } // End while
  // Pop any remaining operators and append them to postfix
  while (!operator_stack.empty()) {
    char op = operator_stack.top();
    operator_stack.pop();
    if (op == '(' || op == '[' || op == '}') {
      throw EvaluatorError("Unmatched open parenthesis");
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
      throw EvaluatorError("Unmatched close parenthesis");
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
          throw EvaluatorError("Unmatched close parentheses");
        }
      } else if (op == ']') {
        if (!operator_stack.empty() 
            && (operator_stack.top() == '[')) {
          operator_stack.pop();
        } else {
          throw EvaluatorError("Unmatched close parentheses");
        }
      } else if (op == '}') {
        if (!operator_stack.empty() 
            && (operator_stack.top() == '{')) {
          operator_stack.pop();
        } else {
          throw EvaluatorError("Unmatched close parentheses");
        }
      } else {
        operator_stack.push(op);
      }
    }
  }
}
