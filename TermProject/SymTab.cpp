//
//		Implementation of the symbol table class.  This is the format I want for commenting functions.
//
#include "stdafx.h"
#include "SymTab.h"

/**/
/*
SymbolTable::AddSymbol()

NAME

	AddSymbol - adds a new symbol to the symbol table.

SYNOPSIS

	void AddSymbol( const string &a_symbol, int a_loc );
		a_symbol	-> The name of the symbol to be added to the symbol table.
		a_loc		-> the location to be associated with the symbol.

DESCRIPTION

	This function will place the symbol "a_symbol" and its location "a_loc"
	in the symbol table.

RETURNS
	Void
*/
/**/
void SymbolTable::AddSymbol(const string& a_symbol, int a_loc)
{
	// If the symbol is already in the symbol table, record it as multiply defined.
	map<string, int>::iterator st = m_symbolTable.find(a_symbol);
	if (st != m_symbolTable.end()) {
		st->second = multiplyDefinedSymbol;
		return;
	}
	// Record a the  location in the symbol table.
	m_symbolTable[a_symbol] = a_loc;
}

/**/
/*
SymbolTable::DisplaySymbolTable()

NAME
	DisplaySymbolTable - displays the symbol table.

SYNOPSIS
	void DisplaySymbolTable();

DESCRIPTION
	This function will display the symbol table.

RETURNS
	Void

*/
/**/
void SymbolTable::DisplaySymbolTable()
{
	cout << "Output from test program\n" << endl;
	cout << "Symbol Table:\n" << endl;
	cout << left << setw(10) << "Symbol#" << setw(10) << "Symbol" << setw(10) << "Location" << endl;

	int i = 0;
	map<string, int>::iterator it;

	for (it = m_symbolTable.begin(); it != m_symbolTable.end(); it++) {
		cout << left << setw(10) << i << setw(10) << it->first << setw(10) << it->second << endl;
		i++;
	}
	cout << endl;
	cout << "____________________________________________________" << endl;
}

/*

NAME

	LookupSymbol - looks up a symbol in the symbol table.

SYNOPSIS

	bool LookupSymbol(const string& a_symbol, int& a_loc);
		a_symbol	-> The name of the symbol to be looked up in the symbol table.
		a_loc		-> the location to be associated with the symbol.

DESCRIPTION
	This function will look up the symbol "a_symbol" in the symbol table.
	If the symbol is found, the function will return true and the location
	of the symbol will be returned in "a_loc".  If the symbol is not found,
	the function will return false.

RETURNS
	True if the symbol is found in the symbol table, false otherwise.
*/

bool SymbolTable::LookupSymbol(const string& a_symbol, int& a_loc)
{
	for (auto it = m_symbolTable.begin(); it != m_symbolTable.end(); it++)
	{
		if (it->first == a_symbol) {
			a_loc = m_symbolTable[a_symbol];
			return true;
		}
	}
	return false;
}