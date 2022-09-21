//
//      Implementation of the Error class.
//

#include "stdafx.h"
#include "Errors.h"

vector<string> Errors::m_ErrorMsgs;


/**/
/*
Errors::InitErrorReporting()

NAME

		Errors::InitErrorReporting()

SYNOPSIS

		void Errors::InitErrorReporting()

DESCRIPTION

		This function empties the queue for error message to prepare it for next execution

RETURNS

		Returns nothing


*/
/**/
// Initializes error reports.
void Errors::InitErrorReporting() 
{
	if (!m_ErrorMsgs.empty()) // if vector is not empty
	{
		while (!m_ErrorMsgs.empty())
		{
			m_ErrorMsgs.pop_back();
		}
	}
}

/**/
/*
Errors::RecordError()

NAME

		Errors::RecordError()

SYNOPSIS

		void Errors::RecordError(string a_emsg)
		a_emsg		--> Error message to be pushed into the vector

DESCRIPTION

		This function records the error message into the m_ErrorMsgs vector

RETURNS

		Returns nothing


*/
/**/
// Records an error message.
void Errors::RecordError(string a_emsg)
{
	//put error message in vector
	m_ErrorMsgs.push_back(a_emsg);
}

/**/
/*
Errors::DisplayErrors()

NAME

		Errors::DisplayErrors()

SYNOPSIS

		void Errors::DisplayErrors()

DESCRIPTION

		This function displays all of the errors in the m_ErrorMsgs vector
		empties m_ErrorMsgs vector for the next line

RETURNS

		Returns nothing


*/
/**/
// Displays the collected error message.
void Errors::DisplayErrors()
{
	//display all error messages for current line
	for (string x : m_ErrorMsgs)
	{
		cout << x << endl;
	}
	// clear error vector for next line
	m_ErrorMsgs.clear();

}