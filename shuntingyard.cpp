#include "shuntingyard.h"

shuntingyard::shuntingyard() {
	init_operators();
}

//infix is your conditions
Queue<string> shuntingyard::infix_to_postfix(vector<string> infix) {
	int i = 0;

	while (i < infix.size()) { //as long as the index is less than the infix.size() to not go out of bounds
		//check if the token that you're reading is NOT an operator
		/*
		* this is for the infix[i] == ")"
			while the operator at the top of the operator stack is not a left parenthesis :
				pop the operator from the operator stack onto the output queue.
			//If the stack runs out without finding a left parenthesis, then there are mismatched parentheses.
			if there is a left parenthesis at the top of the operator stack, then :
				pop the operator from the operator stackand discard it
				if there is a function token at the top of the operator stack, then :
					pop the function from the operator stack onto the output queue.
		*/
		
		if (infix[i] == "(") { //if it's a left paren, just push it into the operator stack
			operator_s.push(infix[i]);
			i++;
		} else if (infix[i] == ")") { //if it's a right paren, that means we've encounter some type of closing paren
			while (!operator_s.empty() && operator_s.top() != "(") { //go until we see (
				string output = operator_s.pop();
				output_q.push(output);
			}
			if (operator_s.top() == "(") { //once we see this, then we can pop
				operator_s.pop();
			}
			i++;
		}
		else if (!_operators.contains(uppercase(infix[i]))) { //IF IT DOES NOT contain one of the infix tokens
			//push into the ouput queue
			output_q.push(infix[i]);
			i++;
		} else { //this means that there in the infix vector theres an operator
			/*
			while ((there is an operator at the top of the operator stack)
              and ((the operator at the top of the operator stack has greater precedence)
                  or (the operator at the top of the operator stack has equal precedence and the token is left associative)):
			*/
			//as long as the op_stack is not empty && as long as top of stack has greater OR equal precendence than the token
			while (!operator_s.empty() && 
				_operators[uppercase(operator_s.top())] >= _operators[uppercase(infix[i])]) {
				//push into the queue with the popped value from operators
				output_q.push(operator_s.pop());
			}
			//otherwise, just push into the stack to wait for next token that's biggest
			operator_s.push(infix[i]);
			i++;
		}
	}
	//when there are no more tokens to look for, pop everything in the stack and push into queue
	while (!operator_s.empty()) {
		output_q.push(operator_s.pop());
	}
	return output_q;
}

//initializes all the operators
void shuntingyard::init_operators() {
	_operators["="] = EQUAL;
	_operators[">="] = GREATER_THAN_EQUAL;
	_operators[">"] = GREATER_THAN;
	_operators["<"] = LESS_THAN;
	_operators["<="] = LESS_THAN_EQUAL;
	_operators["OR"] = OR;
	_operators["AND"] = AND;
	_operators["("] = LEFT_PAREN;
	_operators[")"] = RIGHT_PAREN;
}
