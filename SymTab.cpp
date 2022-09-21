//
//		Implementation of the symbol table class.  This is the format I want for commenting functions.
//
#include "stdafx.h"
#include "SymTab.h"
#include <iomanip>

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

        Returns nothing
*/
/**/
void 
SymbolTable::AddSymbol( const string &a_symbol, int a_loc )
{
    // If the symbol is already in the symbol table, record it as multiply defined.
    map<string, int>::iterator st = m_symbolTable.find( a_symbol );
    if( st != m_symbolTable.end() ) {

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

    SymbolTable::DisplaySymbolTable()

SYNOPSIS

    SymbolTable::DisplaySymbolTable()

DESCRIPTION

    This function displays the symbol table

RETURNS

        Returns nothing
*/
/**/
void 
SymbolTable::DisplaySymbolTable()
{
    int ctr = 0;
    //cout << "Must implement: DisplaySymbolTable( )" << endl;

    map<string, int>::iterator st;
    cout << "Symbol #   Symbol  Location" << endl;
    for (st = m_symbolTable.begin(); st != m_symbolTable.end(); st++)
    {
        cout << setw(11) << left << ctr++ << setw(6) << left << st->first << "  " << setw(30) << left << st->second << endl;
    }
    


    return;
}

/**/
/*
SymbolTable::LookupSymbol()

NAME

    SymbolTable::LookupSymbol()

SYNOPSIS

    bool SymbolTable::LookupSymbol(const string& a_symbol, int& a_loc)
    a_symbol        --> the symbol to be looked for in the symbol table
    a_loc           --> the location of a_symbol

DESCRIPTION

    This function looks for the given symbol in the symbol table

RETURNS

        Returns true if symbol is in the symbol table, false if not
*/
/**/
bool 
SymbolTable::LookupSymbol(const string& a_symbol, int& a_loc)
{
    //cout << "Must implement: LookupSymbol( )" << endl; 
    if ((m_symbolTable.find(a_symbol)) != m_symbolTable.end()) // if symbol is in symbol table
    {
        return true;
    }
    else
    {
        return false;
    }
}

/**/
/*
SymbolTable::LookupLocation()

NAME

    SymbolTable::LookupLocation()

SYNOPSIS

    string SymbolTable::LookupLocation(string& a_symbol)
    a_symbol        --> the symbol to be looked for in the symbol table

DESCRIPTION

    This function looks for the given symbol in the symbol table

RETURNS

        Returns the location of the symbol as a string
*/
/**/
string
SymbolTable::LookupLocation(string& a_symbol)
{
    auto itr = m_symbolTable.find(a_symbol);
    int loc = itr->second;

    return to_string(loc);
}

