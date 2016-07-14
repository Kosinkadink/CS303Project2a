/** Implementation of the postfix_evaluator. */

#include "EvaluatorError.h"
#include "Postfix_Evaluator.h"
#include <sstream>
#include <cctype>
#include <iostream>
using std::stack;
using std::string;
using std::istringstream;
using std::isdigit;

const std::string Postfix_Evaluator::OPERATORS = "!t_~^*/%+->g<len&|";

/** Evaluates a postfix expression.
    @param expression The expression to be evaluated
    @return The value of the expression
    @throws Syntax_Error if a syntax error is detected
*/
int Postfix_Evaluator::eval(const std::string& expression) {
  // Be sure the stack is empty
  while (!operand_stack.empty())
    operand_stack.pop();

  // Process each token
  istringstream tokens(expression);
  char next_char;
  while (tokens >> next_char) {
    if (isdigit(next_char)) {
      tokens.putback(next_char);
      int value;
      tokens >> value;
      operand_stack.push(value);
    } else if (is_operator(next_char)) {
      int result = eval_op(next_char);
      operand_stack.push(result);
    } else {
      throw Syntax_Error("Invalid character encountered");
    }
  }
  if (!operand_stack.empty()) {
    int answer = operand_stack.top();
    operand_stack.pop();
    if (operand_stack.empty()) {
      return answer;
    } else {
      throw Syntax_Error("Stack should be empty");
    }
  } else {
    throw Syntax_Error("Stack is empty");
  }
}

/** Evaluates the current operator.
    This function pops the two operands off the operand
    stack and applies the operator.
    @param op A character representing the operator
    @throws Syntax_Error if top is attempted on an empty stack
*/
int Postfix_Evaluator::eval_op(char op) {
  if (operand_stack.empty()) 
    throw Syntax_Error("Stack is empty");
  int rhs = operand_stack.top();
  operand_stack.pop();
  switch (op) {
  case '~' : return -rhs;
  case '!': return (rhs == 0);
  case 't': return rhs+1;
  case '_': return rhs-1;
  default: break;
  }


  if (operand_stack.empty())
    throw Syntax_Error("Stack is empty");
  int lhs = operand_stack.top();
  int result = 0;
  operand_stack.pop();
  try{
  switch(op) {
  case '+' : 
             result = lhs + rhs;
             break;
  case '-' : result = lhs - rhs;
             break;
  case '*' : result = lhs * rhs;
             break;
             
  case '/':
      if (rhs != 0){
          result = lhs / rhs;
          break;
      }
      else
          throw EvaluatorError("Division by zero is not allowed.");
             

  case '%' : result = lhs % rhs;
             break;
  case '^': result = pow(lhs, rhs); break;
  case '>': result = lhs > rhs; break;
  case 'g': result = lhs >= rhs; break;
  case '<': result = lhs < rhs; break;
  case 'l': result = lhs <= rhs; break;
  case 'e': result = lhs == rhs; break;
  case 'n': result = lhs != rhs; break;
  case '&': result = lhs && rhs; break;
  case '|': result = lhs || rhs; break;
  }
  }
  catch (EvaluatorError error){
      cout << error.what() << endl;
      system("pause");
  }
  return result;
}
