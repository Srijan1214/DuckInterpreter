#pragma once
class Statement
{
public:
	Statement();
	~Statement();

	void RecordStatements(string a_sourceFileName);
	string GetStatement(int a_statementNum) {
		if (a_statementNum >= m_statements.size()) 
		{
			cout << "Read Program Successfully" << endl;
			exit(EXIT_SUCCESS);
		}
		return m_statements[a_statementNum];
	}
	int GetLabelLocation(string a_string);

	//My function
	bool check_if_statements_contain_stop_keyword();
	bool check_if_statements_after_end_keyword();

private:
	vector<string> m_statements;
	map<string, int> m_labelToStatement;
	void put_raw_lines_from_file_to_vector(string a_sourceFileName);
	void remove_comments();
	void put_labels_from_vector_into_map();
	void remove_labels_from_vector();
	void trim_elements_in_vector();
	void remove_break_line_character();
	void trim_string(string &temp);
};