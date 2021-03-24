#pragma once
#include <iostream>
#include <vector>

#include "Queue.h"
#include "Stack.h"
#include "Map.h"
#include "string_utility.h"

using namespace std;

class shuntingyard {
public:
	shuntingyard();
	Queue<string> infix_to_postfix(vector<string> infix);

private:
	void init_operators();
	Map<string, int> _operators;
	Stack<string> operator_s; //stack for operators
	Queue<string> output_q; //queue for everything else
};

enum OPERATORS {
	OR = 1,
	AND = 2,
	GREATER_THAN = 3,
	GREATER_THAN_EQUAL = 3,
	LESS_THAN = 3,
	LESS_THAN_EQUAL = 3,
	EQUAL = 3,
	LEFT_PAREN = 0,
	RIGHT_PAREN = 0
};