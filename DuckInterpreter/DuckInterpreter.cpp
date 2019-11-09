#include "stdafx.h"
#include "DuckInterpreter.h"


DuckInterpreter::DuckInterpreter()
{
}

DuckInterpreter::~DuckInterpreter()
{
}

void DuckInterpreter::RunInterpreter()
{
	int nextStatement = 0;

	if (!m_statements.check_if_statements_contain_stop_keyword())
	{
		cerr << "No stop Statement in File(ERROR)" << endl;
		exit(EXIT_FAILURE);
	}
	if (m_statements.check_if_statements_after_end_keyword())
	{
		cerr << "Statements after end keyword in File(ERROR)" << endl;
		exit(EXIT_FAILURE);
	}
	while (true)
	{
		string statement = m_statements.GetStatement(nextStatement);

		nextStatement = ExecuteStatement(statement, nextStatement);
	}
}

int DuckInterpreter::ExecuteStatement(string a_statement, int a_nextStatement)
{
	// Clear the stacks
	m_numberStack.clear();
	m_operatorStack.clear();
	int nextElement = 0;

	if ("" == a_statement)
	{
		return a_nextStatement + 1;
	}

	// Get the first syntactic element of the statement.  
	StatementType type = GetStatementStype(a_statement);

	// Based on the type, execute the remainder of the statement.
	switch (type)
	{
	case StatementType::ArithmeticStat:
		EvaluateArithmentStatment(a_statement);
		return a_nextStatement + 1;

	case StatementType::IfStat:
		return EvaluateIfStatement(a_statement, a_nextStatement);
	case StatementType::StopStat:
		exit(EXIT_FAILURE);
	case StatementType::EndStat:
		exit(EXIT_SUCCESS);
	case StatementType::PrintStat:
		return EvaluatePrintStatement(a_statement, a_nextStatement);
	case StatementType::ReadStat:
		return EvaluateReadStatement(a_statement, a_nextStatement);
	case StatementType::gotoStat:
		return EvaluateGotoStatement(a_statement, a_nextStatement);
	default:
		cerr << "BUGBUG - program terminate: invalid return value from GetStatementStype for the statement: " << a_statement << endl;
		exit(1);
	}
}
// we know at this point that we have an arithementic expression.  Excute this statement.  Any error
// will perminate the program.

/*
DuckInterpreter::EvaluateArithmentStatment()

NAME

		DuckInterpreter::EvaluateArithmentStatment - evaluates arithmetics statements.

SYNOPSIS

		DuckInterpreter::EvaluateArithmentStatment(string a_statement, int a_nextStatement);
			a_statement             --> the current line of text

DESCRIPTION

	This function has the resposibility of handling the arithmetic statement type of the project. It is the part reposnsibe for assignments.

RETURNS

	Returns the next line of the program.

AUTHOR

		Srijan Joshi

*/
/**/
void DuckInterpreter::EvaluateArithmentStatment(const string &a_statement)
{
	// Record the variable that we will be assignning a value.
	int nextPos = 0;
	string resultVariable;
	double placeHolder;
	nextPos = ParseNextElement(a_statement, nextPos, resultVariable, placeHolder);
	//assert(!resultVariable.empty());
	trim_string(resultVariable);


	string assigmentOp;
	nextPos = ParseNextElement(a_statement, nextPos, assigmentOp, placeHolder);
	//assert(assigmentOp != "=");

	double result = EvaluateArithmenticExpression(a_statement, nextPos);

	// Record the result.
	m_symbolTable.RecordVariableValue(resultVariable, result);

}
/*void DuckInterpreter::EvaluateArithmentStatment(const string &a_statement);
*/




// Evaluates an if statement to determine if the goto should be executed.
/**/
/*
DuckInterpreter::EvaluateIfStatement() spmLongShort::ProcessNewOpens()

NAME

		DuckInterpreter::EvaluateIfStatement - evaluates if statements.

SYNOPSIS

		DuckInterpreter::EvaluateIfStatement(string a_statement, int a_nextStatement);
			a_statement             --> the current line of text

DESCRIPTION

	This function has the resposibility of handling the if statement type of the project.

RETURNS

	Returns the next line if condition is false. Else, it returns the line number pointed by the goto statement.

AUTHOR

		Srijan Joshi

*/
/**/
int DuckInterpreter::EvaluateIfStatement(string a_statement, int a_nextStatement)
{
	// Get past of the "if" in the if statement.
	int nextPos = 0;
	string resultString;
	double placeHolder;
	nextPos = ParseNextElement(a_statement, nextPos, resultString, placeHolder);
	assert(resultString == "if");

	// Searching from the end, fine the goto statement and replace it by ";"  Record
	// the label in the goto.

	string label=a_statement;
	label.replace(a_statement.find("goto"), 5, "");
	label = label.substr(label.find_last_of(")")+1, label.size() - label.find_last_of(")"));
	trim_string(label);


	int labelLocation = m_statements.GetLabelLocation(label);

	// Verify that the label from the goto exists.
	if (labelLocation == -1)
	{
		cerr << "Invalid Label" << endl;
		exit(EXIT_FAILURE);
	}

	// Evaluate the remaining arithmentic expression. 
	double result = EvaluateArithmenticExpression(a_statement.substr(0,a_statement.find_last_of(")")+1), nextPos);

	// If the result is zero, don't execute the goto.
	if (result == 0)
	{
		return a_nextStatement + 1;
	}
	return labelLocation;

}
/*int DuckInterpreter::EvaluateIfStatement(string a_statement, int a_nextStatement);*/





/**/
/*
DuckInterpreter::EvaluatePrintStatement()

NAME

		DuckInterpreter::EvaluatePrintStatement - evaluates print statements in the interpreter.

SYNOPSIS

		DuckInterpreter::EvaluatePrintStatement(string a_statement, int a_nextStatement)
			a_statement             --> the current line of text
			a_nextStatement        --> current_line number

DESCRIPTION

	This function has the resposibility of handling the print statements type of the project.

RETURNS

	Returns the next line of the program.

AUTHOR

		Srijan Joshi

*/
/**/
int DuckInterpreter::EvaluatePrintStatement(string a_statement, int a_nextStatement) 
{
	vector<string> output_buffer;
	string temp = a_statement;
	temp.replace(a_statement.find("print"), 5, "");
	trim_string(temp);

	parse_strings_into_vector(output_buffer, temp);

	double value;
	for (int i = 0; i < output_buffer.size(); i++)
	{
		if ((output_buffer[i])[0] == '\"') 
		{
			cout << output_buffer[i].substr(1, output_buffer[i].size() - 2);
		}
		else
		{
			if (m_symbolTable.GetVariableValue(output_buffer[i], value)) 
			{
				cerr << "Undefined Variable" << endl;
				exit(EXIT_FAILURE);
			}
			else
			{
				cout << value;
			}
		}
	}
	cout << endl;
	return a_nextStatement + 1;
}
/*int DuckInterpreter::EvaluatePrintStatement(string a_statement, int a_nextStatement);*/





/**/
/*
DuckInterpreter::EvaluateReadStatement()

NAME

		DuckInterpreter::EvaluateReadStatement - evaluates read statements in the interpreter.

SYNOPSIS

		DuckInterpreter::EvaluateReadStatement(string a_statement, int a_nextStatement)
			a_statement             --> the current line of text
			a_nextStatement        --> current_line number

DESCRIPTION

	This function has the resposibility of handling the read statements type of the project.

RETURNS

	Returns the next line of the program.

AUTHOR

		Srijan Joshi

*/
/**/
int DuckInterpreter::EvaluateReadStatement(string a_statement, int a_nextStatement) 
{
	vector<string> input_buffer;
	string output_prompt;
	string temp = a_statement;
	temp = temp.substr(temp.find_first_of(",") + 1, temp.size() - 1);
	trim_string(temp);
	output_prompt = a_statement.substr(a_statement.find_first_of("\"") + 1, a_statement.find_last_of("\"") - a_statement.find_first_of("\"")-1);
	cout << output_prompt << endl;

	parse_strings_into_vector(input_buffer, temp);

	double value;
	for (int i = 0; i < input_buffer.size(); i++)
	{
		{
			cin >> value;
			m_symbolTable.RecordVariableValue(input_buffer[i], value);

		}
	}
	return a_nextStatement + 1;
}
/*int DuckInterpreter::EvaluateReadStatement(string a_statement, int a_nextStatement) 
*/







/*
DuckInterpreter::ParseNextElement()

NAME

		DuckInterpreter::ParseNextElement - A multitasking function that gives the starting position of next element in the statment.


SYNOPSIS

		DuckInterpreter::ParseNextElement(const string &a_statement, int a_nextPos, string &a_stringValue, double &numValue)
			a_statement             --> the current line of text
			a_nextPos				-->the current starting position.
			a_stringValue			--> A variable that gets the next element if the next element is a string.
			numValue				--> A variable that gets the next element if the next element is a number.

DESCRIPTION

	This function has the resposibility of giving the next element to itself.

RETURNS

	Returns the starting search position of the next element of the line.

AUTHOR

		Srijan Joshi

*/
/**/
int DuckInterpreter::ParseNextElement(const string &a_statement, int a_nextPos, string &a_stringValue, double &numValue)
{
	if (a_nextPos == 0) 
	{
		int ret_val;
		ret_val= handle_keywords_for_parsing_elements(a_statement, a_nextPos, a_stringValue, numValue);
		if (ret_val != -1) 
		{
			return ret_val;
		}
	}
	if (check_if_satisfactory_starting_character_for_variable(a_statement[a_nextPos]))
	{
		return handle_variables_for_parsing_elements(a_statement, a_nextPos, a_stringValue, numValue);
	}
	if (a_statement[a_nextPos] == '=') 
	{
		a_stringValue = a_statement[a_nextPos];
		return a_nextPos+1;
	}
	return -1;
}
/*int DuckInterpreter::ParseNextElement(const string &a_statement, int a_nextPos, string &a_stringValue, double &numValue)*/





/*
DuckInterpreter::EvaluateGotoStatement()

NAME

		DuckInterpreter::EvaluateGotoStatement - A multitasking function that gives the starting position of next element in the statment.


SYNOPSIS

		DuckInterpreter::EvaluateGotoStatement(string a_statement, int a_nextStatement)
			a_statement             --> the current line of text
			a_nextStatement				-->the current starting position.

DESCRIPTION

	This function has the resposibility of evaluating the goto statements. Stops the program is the label does not exits.

RETURNS

	Returns the line number of the label if the label exits.

AUTHOR

		Srijan Joshi

*/
/**/
int DuckInterpreter::EvaluateGotoStatement(string a_statement, int a_nextStatement)
{
	string label = a_statement;
	label.replace(a_statement.find("goto"), 5, "");
	label = label.substr(label.find_last_of(")") + 1, label.size() - label.find_last_of(")"));
	trim_string(label);

	int labelLocation = m_statements.GetLabelLocation(label);
	trim_string(label);
	if (labelLocation != -1) {
		return labelLocation;
	}
	else
	{
		cerr << "Invalid Label" << endl;
		exit(EXIT_FAILURE);
	}
}
/*int DuckInterpreter::EvaluateGotoStatement(string a_statement, int a_nextStatement)
*/





/*
DuckInterpreter::handle_variables_for_parsing_elements()

NAME

		DuckInterpreter::handle_variables_for_parsing_elements - A sub function for ParseNextElement that handles everything related to variables..


SYNOPSIS

		DuckInterpreter::handle_variables_for_parsing_elements(const string &a_statement, int a_nextPos, string &a_stringValue, double &numValue)
			a_statement             --> the current line of text
			a_nextPos				-->the current starting position.
			a_stringValue			--> A variable that gets the next element if the next element is a string.
			numValue				--> A variable that gets the next element if the next element is a number.

DESCRIPTION

	This function handles the variables elements when elements are parsed in the ParseNextElement function.

RETURNS

	Returns the starting search position of the next element of the line.

AUTHOR

		Srijan Joshi

*/
/**/
int DuckInterpreter::handle_variables_for_parsing_elements(const string &a_statement, int a_nextPos, string &a_stringValue, double &numValue) 
{
	int temp_next_pos;
	if (check_if_satisfactory_starting_character_for_variable(a_statement[a_nextPos]))
	{
		temp_next_pos = a_statement.find_first_of('=', a_nextPos);
		if (temp_next_pos != std::string::npos) 
		{
			a_stringValue = a_statement.substr(a_nextPos, temp_next_pos);
			while (a_statement[temp_next_pos+1] == '\t' || a_statement[temp_next_pos] == ' ')
			{
				temp_next_pos++;
			}
			return temp_next_pos;
		}
	}

	return -1;
}
/*int DuckInterpreter::handle_variables_for_parsing_elements(const string &a_statement, int a_nextPos, string &a_stringValue, double &numValue);
*/





/*
DuckInterpreter::handle_keywords_for_parsing_elements()

NAME

		DuckInterpreter::handle_keywords_for_parsing_elements - A sub function for ParseNextElement that handles everything related to variables..


SYNOPSIS

		DuckInterpreter::handle_keywords_for_parsing_elements(const string &a_statement, int a_nextPos, string &a_stringValue, double &numValue)
			a_statement             --> the current line of text
			a_nextPos				-->the current starting position.
			a_stringValue			--> A variable that gets the next element if the next element is a string.
			numValue				--> A variable that gets the next element if the next element is a number.

DESCRIPTION

	This function handles the keywords like "if","read",e.t.c, if they appear to be an element. elements when elements are parsed in the ParseNextElement function.

RETURNS

	Returns the starting search position of the next element of the line.

AUTHOR

		Srijan Joshi

*/
/**/
int DuckInterpreter::handle_keywords_for_parsing_elements(const string &a_statement, int a_nextPos, string &a_stringValue, double &numValue) 
{
		if ("if" == a_statement.substr(a_nextPos, 2)) 
		{
			a_stringValue = "if";
			a_nextPos = a_statement.find("(");
			return a_nextPos;
		}
		if ("read" == a_statement.substr(a_nextPos, 4)) 
		{
			a_stringValue = "read";
			return 4;
		}
		if ("print" == a_statement.substr(a_nextPos, 5)) 
		{
			a_stringValue = "print";
			return 5;
		}
		if ("stop" == a_statement.substr(a_nextPos, 4)) 
		{
			a_stringValue = "stop";
			return 4;
		}
		if ("end" == a_statement.substr(a_nextPos, 3)) 
		{
			a_stringValue = "end";
			return 3;
		}
		return -1;
}
/*int DuckInterpreter::handle_keywords_for_parsing_elements(const string &a_statement, int a_nextPos, string &a_stringValue, double &numValue);
*/

//Checks if the chars can be the first letter of a variable.
bool DuckInterpreter::check_if_satisfactory_starting_character_for_variable(const char &a_starting_char)
{
	if (a_starting_char == '_' || isalpha(a_starting_char))
	{
		return true;
	}
	return false;
}
/*bool DuckInterpreter::check_if_satisfactory_starting_character_for_variable(const char &a_starting_char);
*/

//Returns the precedence of the oprattors
int DuckInterpreter::FindPrecedence(char op)
{
	if(op=='>'||op=='<')
	{
		return 1;
	}
	else if (op == '+' || op == '-') {
		return 2;
	}
	else if (op == '*' || op == '/') {
		return 3;
	}
	else if (op == '^') {
		return 4;
	}
	else {
		return 0;
	}
}
/*int DuckInterpreter::FindPrecedence(char op);
*/






/*
DuckInterpreter::EvaluateArithmenticExpression()

NAME

		DuckInterpreter::EvaluateArithmenticExpression - Function responible for handling arithmetic expressions in the file.


SYNOPSIS

		DuckInterpreter::EvaluateArithmenticExpression(const string &a_statement, int a_nextPos, string &a_stringValue, double &numValue)
			a_statement             --> the current line of text
			a_nextPos				-->the current line number.


DESCRIPTION

	Arithmatic expression. These expressions are handled by using stacks and the Reverse Polish Notation. Other functions like give_postfix_of_expression() and give_postfix_of_expression() help implement RNF.

RETURNS

	Returns the starting search position of the next element of the line.

AUTHOR

		Srijan Joshi

*/
/**/
double DuckInterpreter::EvaluateArithmenticExpression(const string &a_statement, int a_nextPos) 
{
	string temp = a_statement.substr(a_nextPos,a_statement.size()-a_nextPos);
	trim_string(temp);
	if (!check_if_expression_contains_variables(temp))
	{
		return evaluate_expression(temp);
	}
	else 
	{
		string var="";
		double var_value;
		int pos = 0;
		do
		{
			pos = 0;
			var = get_available_variable_in_expression_from_position(temp, pos);
			if (m_symbolTable.GetVariableValue(var, var_value)) {
				cerr << "Undefined Variable" << endl;
				exit(EXIT_FAILURE);
			}
			if (var != "") 
			{
				replace_variable_with_value_in_expression(temp, var, var_value);
			}
		} while (pos!=-1);
		return evaluate_expression(temp);
	}
}
/*double DuckInterpreter::EvaluateArithmenticExpression(const string &a_statement, int a_nextPos); 
*/



/*
DuckInterpreter::StatementType()

NAME

		DuckInterpreter::StatementType -Returns one of the enum types of statements.


SYNOPSIS

		DuckInterpreter::GetStatementStype(const string &a_string)
			a_string		--> the current line
DESCRIPTION

	This function maily looks at the keywords like "if" and read to figure out what each statement type is.

RETURNS

	Returns the starting search position of the next element of the line.

AUTHOR

		Srijan Joshi

*/
/**/
DuckInterpreter::StatementType DuckInterpreter::GetStatementStype(const string &a_string) {
	if (a_string.substr(0, 4) == "read") 
	{
		return StatementType::ReadStat;
	}else if (a_string.substr(0, 4) == "stop")
	{
		return StatementType::StopStat;
	}else if (a_string.substr(0, 3) == "end")
	{
		return StatementType::EndStat;
	}else if (a_string.substr(0, 5) == "print")
	{
		return StatementType::PrintStat;
	}else if (a_string.substr(0, 2) == "if")
	{
		return StatementType::IfStat;
	}
	else if (a_string.substr(0, 4) == "goto")
	{
		return StatementType::gotoStat;
	}
	else {
		return StatementType::ArithmeticStat;
	}
}
/*DuckInterpreter::StatementType DuckInterpreter::GetStatementStype(const string &a_string) {
*/





/*
DuckInterpreter::perform_arithmetic_action(const char &op, const double &num1, const double& num2)
NAME

		DuckInterpreter::perform_arithmetic_action(const char &op, const double &num1, const double& num2) - Performs arithmetic operation based on the operator.


SYNOPSIS
DuckInterpreter::perform_arithmetic_action(const char &op, const double &num1, const double& num2)
			op             --> the current line of text
			num1				-->the current starting position.
			num2			--> A variable that gets the next element if the next element is a string.

DESCRIPTION

	Performs arithmetic operation based on the operator.

RETURNS

	Returns the starting search position of the next element of the line.

AUTHOR

		Srijan Joshi

*/
/**/
double DuckInterpreter::perform_arithmetic_action(const char &op, const double &num1, const double& num2) {
	switch (op)
	{
	case '*':
		return num1 * num2;
		break;
	case '/':
		return num2 / num1;
		break;
	case '+':
		return num1 + num2;
		break;
	case '-':
		return num2 - num1;
		break;	
	case '<':
		return num2 < num1;
		break;
	case '>':
		return num2 > num1;
		break;

	default:
		return 0;
		break;
	}
}
/*double DuckInterpreter::perform_arithmetic_action(const char &op, const double &num1, const double& num2) {
*/









/*
DuckInterpreter::give_postfix_of_expression(const string &a_expression)
NAME

		DuckInterpreter::give_postfix_of_expression(const string &a_expression) - Performs arithmetic operation based on the operator.


SYNOPSIS
DuckInterpreter::give_postfix_of_expression(const string &a_expression)
		expression		-->the current expresion that should be in pre fix order.

DESCRIPTION

	Performs arithmetic operation based on the operator.

RETURNS

	Returns a string that is in EPN/ INfix order.

AUTHOR

		Srijan Joshi

*/
/**/
string DuckInterpreter::give_postfix_of_expression(const string &a_expression) {
	string postfix = ""; // Initialize postfix as empty string.
	for (int i = 0; i < a_expression.length(); i++) {

		// Scanning each character from left. 
		// If character is a delimitter, move on. 
		if (a_expression[i] == ' ' || a_expression[i] == '\t') continue;

		// If character is operator, pop two elements from stack, perform operation and push the result back. 
		else if (isoperator(a_expression[i]))
		{
			while (!m_numberStack.empty() && m_numberStack.back() != '(' && (FindPrecedence(m_numberStack.back() > FindPrecedence(a_expression[i]))))
			{
				//postfix += m_numberStack.back();
				postfix = postfix + m_operatorStack.back() + ' ';
				m_numberStack.pop_back();
			}
			m_numberStack.push_back(a_expression[i]);
		}
		// Else if character is an operand
		else if (isoperand(a_expression[i]))
		{
			string temp = "";
			while (isoperand(a_expression[i]))
			{
				temp = temp + a_expression[i];
				i++;
			}
			postfix = postfix + temp + ' ';
			i--;
		}
		else if (a_expression[i] == '(')
		{
			m_numberStack.push_back(a_expression[i]);
		}

		else if (a_expression[i] == ')')
		{
			while (!m_numberStack.empty() && m_numberStack.back() != '(') 
			{
				postfix += m_numberStack.back();
				m_numberStack.pop_back();
			}
			m_numberStack.pop_back();
		}
	}

	while (!m_numberStack.empty()) 
	{
		postfix += m_numberStack.back();
		m_numberStack.pop_back();
	}

	return postfix;

	/*string DuckInterpreter::give_postfix_of_expression(const string &a_expression) {
*/

}
/*string DuckInterpreter::give_postfix_of_expression(const string &a_expression)*/






/*
DuckInterpreter::evaluatePostfix(const string &a_expression)
NAME

		DuckInterpreter::evaluatePostfix(const string &a_expression) - Evaluates the postfix string using stacks.


SYNOPSIS
DuckInterpreter::evaluatePostfix(const string &a_expression)
		expression		-->the current expresion that should be in post fix order.

DESCRIPTION

	Calculates the postfix string using stacks.

RETURNS

	Returns the final computed product.

AUTHOR

		Srijan Joshi

*/
/**/
double DuckInterpreter::evaluatePostfix(string &a_expression)
{
	m_numberStack.clear();
	m_operatorStack.clear();

	for (int i = 0; i < a_expression.size(); ++i)
	{
		if (a_expression[i] != ' ') 
		{
			double temp = 0;
			if (isoperand(a_expression[i]) && (a_expression[i] != ' ')) 
			{
				while (isoperand(a_expression[i]) && (a_expression[i] != ' '))
				{
					temp = temp * 10 + a_expression[i] - 48;
					i++;
				}
				i--;
				m_numberStack.push_back(temp);
			}
			temp = 0;
			if (isoperator(a_expression[i]))
			{
				double num1 = m_numberStack.back();
				m_numberStack.pop_back();
				double num2 = m_numberStack.back();
				m_numberStack.pop_back();
				m_numberStack.push_back(perform_arithmetic_action(a_expression[i], num1, num2));
			}
		}
	}

	return m_numberStack.back();
}
/*double DuckInterpreter::evaluatePostfix(string &a_expression)*/









/*
DuckInterpreter::evaluatePostfix(const string &a_expression)
NAME

		DuckInterpreter::evaluatePostfix(const string &a_expression) - Evaluates any given arithmetic expression.


SYNOPSIS
DuckInterpreter::evaluatePostfix(const string &a_expression)
		expression		-->the current expresion that should be in pre fix order.

DESCRIPTION

	Evaluates any given arithmetic expression.

RETURNS

	Returns the final computed product.

AUTHOR

		Srijan Joshi

*/
/**/
double DuckInterpreter::evaluate_expression(string &a_expression)
{
	string temp = give_postfix_of_expression(a_expression);
	return evaluatePostfix(temp);
}
/*double DuckInterpreter::evaluate_expression(string &a_expression)
*/

bool DuckInterpreter::isoperand(const char& op)
{
	if (op >= '0' && op <= '9') return true;
	if (op >= 'a' && op <= 'z') return true;
	if (op >= 'A' && op <= 'Z') return true;
	return false;
}
/*bool DuckInterpreter::isoperand(const char& op)
*/

bool DuckInterpreter::isoperator(const char& op)
{
	if (op == '+' || op == '-' || op == '*' || op == '/' || op == '<' || op == '>') 
	{
		return true;
	}

	return false;
}
/*bool DuckInterpreter::isoperator(const char& op)
*/

bool DuckInterpreter::check_if_expression_contains_variables(const string &a_expression) 
{
	for (size_t i = 0; i < a_expression.size(); i++)
	{
		if (isalpha(a_expression[i])) 
		{
			return true;
		}
	}
	return false;
}
/*bool DuckInterpreter::check_if_expression_contains_variables(const string &a_expression) 
*/

string DuckInterpreter::get_available_variable_in_expression_from_position(const string& a_expression, int &pos) 
{
	string temp="";
	for (size_t i = pos; i < a_expression.size(); i++)
	{
		if (isalpha(a_expression[i]) || a_expression[i] == '_')
		{
			temp += a_expression[i];
			i++;
			while (isalpha(a_expression[i]))
			{
				temp += a_expression[i];
				i++;
			}
			if (i < a_expression.size())
			{
				pos = i;
			}
			else
			{
				pos = -1;
				return temp;
			}
			return temp;
		}
	}
	pos = -1;
	return temp;
}
/*string DuckInterpreter::get_first_variable_in_expression_from_position(const string& a_expression, int &pos) 
*/

//Function that replaces the given variable in an expression with a string
void DuckInterpreter::replace_variable_with_value_in_expression(string &a_expression, string &variable, double value)
{
	int pos = 0;

	do 
	{
		pos = a_expression.find(variable,pos);
		if (pos != std::string::npos) 
		{
			a_expression.replace(pos, variable.length(),std::to_string((int)value));//need to change to decimal
		}
	} while (pos!=std::string::npos);
}
/*void DuckInterpreter::replace_variable_with_value_in_expression(string &a_expression, string &variable, double value)
*/

//Generic function for trimming any given string
void DuckInterpreter::trim_string(string &a_string) 
{
	while (a_string.size() != 0 && ' ' == a_string[0])
	{
		a_string.erase(0, 1);
	}
	while (a_string.size() != 0 && ' ' == a_string[a_string.size() - 1])
	{
		a_string.erase(a_string.size() - 1);
	}

}
/*void DuckInterpreter::trim_string(string &a_string) 
*/

//Su functions for read and print statement handlers
void DuckInterpreter::parse_strings_into_vector(vector<string> &vec, string a_string)
{
	string delimiter = ",";
	size_t pos = 0;
	string token;
	while ((pos = a_string.find(delimiter)) != std::string::npos) 
	{
		token = a_string.substr(0, pos);
		trim_string(token);
		vec.push_back(token);
		a_string.erase(0, pos + delimiter.length());
	}
	trim_string(a_string);
	vec.push_back(a_string);
}
/*void DuckInterpreter::parse_strings_into_vector(vector<string> &vec, string a_string)
*/
