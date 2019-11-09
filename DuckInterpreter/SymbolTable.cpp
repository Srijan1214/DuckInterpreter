#include "stdafx.h"
#include "SymbolTable.h"

// Might not need constructor and destructor.
SymbolTable::SymbolTable()
{
}


SymbolTable::~SymbolTable()
{
}

bool SymbolTable::GetVariableValue(string a_variable, double &a_value) 
{
		if(m_SymbolTable.find(a_variable)!=m_SymbolTable.end()){
			a_value = m_SymbolTable.find(a_variable)->second;
		return true;
	}
		else 
	{
		return -1;
	}
}