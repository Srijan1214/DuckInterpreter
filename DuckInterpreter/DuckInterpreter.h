#pragma once
#include "Statement.h"
#include "SymbolTable.h"
class DuckInterpreter
{
public:
	DuckInterpreter();
	~DuckInterpreter();

	// Records the statements that are in the specified file.
	void RecordStatements(string a_fileName)
	{
		m_statements.RecordStatements(a_fileName);
	}
	// Runs the interpreter on the recorded statements.
	void RunInterpreter();

private:

	// The statement object that holds the recorded statements.
	Statement m_statements;

	// The sysmbol table object that holds all the variable names and their values.
	SymbolTable m_symbolTable;

	// Stacks for the operators and numbers.  These will be used in evaluating statements.
	vector<char> m_operatorStack;
	vector<double> m_numberStack;


	enum class StatementType
	{
		ArithmeticStat,
		IfStat,
		ReadStat,
		PrintStat,
		StopStat,
		EndStat,
		gotoStat,
	};
	// Determines the type of the next statement.
	StatementType GetStatementStype(const string &a_string);

	enum class ElementType
	{
		StringType,
		NumberType,
	};

	// Executes the statement at the specified location.  Returns the location of the next statement to be executed.
	int ExecuteStatement(string a_statement, int a_StatementLoc);

	// Returns the next element in the statement.  Returns the next location to be accessed.
	int ParseNextElement(const string &a_statement, int a_nextPos, string &a_stringValue, double &numValue);

	// Returns the precedence of an operator.
	int FindPrecedence(char op);

	// Evaluate an arithmetic statement.
	void EvaluateArithmentStatment(const string &a_statement);

	// Evaluate an arithmetic expression.  Return the value.  The variable a_nextPos is index to the next  
	double EvaluateArithmenticExpression(const string &a_statement, int a_nextPos);

	int EvaluateIfStatement(string a_statement, int a_nextStatement);

	//my own added functions
	int EvaluateGotoStatement(string a_statement, int a_nextStatement);
	int EvaluatePrintStatement(string a_statement, int a_nextStatement);
	int EvaluateReadStatement(string a_statement, int a_nextStatement);
	string give_postfix_of_expression(const string &infix);
	bool isoperator(const char &);
	bool isoperand(const char &);
	double evaluatePostfix(string &exp);
	double evaluate_expression(string &exp);
	double perform_arithmetic_action(const char &op, const double &num1, const double& num2);
	bool check_if_expression_contains_variables(const string &exp);
	string get_available_variable_in_expression_from_position(const string& exp, int &pos);
	void replace_variable_with_value_in_expression(string &exp, string &variable, double value);
	void trim_string(string &);
	void parse_strings_into_vector(vector<string> &, string );

	//sub-funtctions to parsenextstatement
	int handle_keywords_for_parsing_elements(const string &a_statement, int a_nextPos, string &a_stringValue, double &numValue);
	int handle_variables_for_parsing_elements(const string &a_statement, int a_nextPos, string &a_stringValue, double &numValue);
	bool check_if_satisfactory_starting_character_for_variable(const char &starting_char);
};