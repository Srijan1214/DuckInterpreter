#include "stdafx.h"
#include "Statement.h"


Statement::Statement()
{
}


Statement::~Statement()
{
}
void Statement::RecordStatements(string a_sourceFileName)
{
		put_raw_lines_from_file_to_vector(a_sourceFileName);
		remove_comments();
		put_labels_from_vector_into_map();
		remove_labels_from_vector();
		trim_elements_in_vector();
		remove_break_line_character();
		trim_elements_in_vector();
		

}

void Statement::put_raw_lines_from_file_to_vector(string a_sourceFileName)
{
	ifstream inf(a_sourceFileName);
	if (!inf)
	{
		cerr << "could not open the file: " << a_sourceFileName << endl;
		exit(1);
	}

	string line;
	if (inf.is_open()) {
		while (getline(inf, line))
		{
			m_statements.push_back(line);//raw line is first pushed
		}
	}
}

void Statement::remove_comments() 
{
	std::size_t pos;
	for (int i = 0; i < m_statements.size(); i++)
	{
		if ((pos = m_statements[i].find("//", 0)) != std::string::npos)
		{
			m_statements[i] = m_statements[i].substr(0, pos);
		}
	}
}

void Statement::put_labels_from_vector_into_map()
{
	for (int i = 0; i < m_statements.size(); i++) 
	{
		std::size_t pos;
		if ((pos = m_statements[i].find_first_of(":", 0)) != std::string::npos)
		{
			if (m_labelToStatement.find(m_statements[i].substr(0, pos)) == m_labelToStatement.end())
			{
				m_labelToStatement.insert(pair<string, int>(m_statements[i].substr(0, pos), i));
			}
			else
			{
				cerr << "Multiply defined Label(ERROR)" << endl;
				exit(EXIT_FAILURE);
			}
		}
	}
}

void Statement::remove_labels_from_vector() {
	std::size_t pos;
	std::size_t temp1;
	std::size_t temp2;

	for (int i = 0; i < m_statements.size(); i++)
	{
		if ((pos = m_statements[i].find_first_of(":", 0)) != std::string::npos)
		{
			if ((temp1 = m_statements[i].find_first_of("\"", 0)) != std::string::npos)
			{
				if(temp1>pos)
					m_statements[i] = m_statements[i].substr(pos + 1);
			}
			else {
				m_statements[i] = m_statements[i].substr(pos + 1);
			}
		}
	}
}

void Statement::trim_elements_in_vector() 
{
	size_t first;
	size_t last;
	string temp;
	for (int i = 0; i < m_statements.size(); i++)
	{
		if (m_statements[i].size() != 0)
		{	
			temp = m_statements[i];
			trim_string(temp);
			m_statements[i] = temp;
		}
	}
}

void Statement::remove_break_line_character()
{
	std::size_t pos;
	for (int i = 0; i < m_statements.size(); i++)
	{
		if ((pos = m_statements[i].find_first_of(";", 0)) != std::string::npos)
		{
			m_statements[i].erase(pos, 1);
		}
	}
}

int Statement::GetLabelLocation(string a_string)
{
	if (!(m_labelToStatement.find(a_string)==m_labelToStatement.end()))
	{
		return m_labelToStatement.find(a_string)->second;
	}
	else 
	{
		return -1;
	}
}

bool Statement::check_if_statements_contain_stop_keyword() 
{
	for (int i = 0; i < m_statements.size(); i++) 
	{
		if ("stop" == m_statements[i])
		{
			return true;
		}
	}
	return false;
}

bool Statement::check_if_statements_after_end_keyword() 
{
	for (int i = 0; i < m_statements.size(); i++)
	{
		if ("end" == m_statements[i])
		{
			if (i != m_statements.size()-1)
			{
				while (i++ < m_statements.size())
				{
					if (m_statements[i].size() != 0)//anything that is not an empty line
					{
						return true;
					}
				}
			}
		}
	}

	return false;
}

void Statement::trim_string(string &temp)
{
		while (temp.size() != 0 && ' ' == temp[0])
		{
			temp.erase(0, 1);
		}
		while (temp.size() != 0 && ' ' == temp[temp.size() - 1])
		{
			temp.erase(temp.size()-1);
		}
}