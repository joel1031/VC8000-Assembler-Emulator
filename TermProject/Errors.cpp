#include "stdafx.h"

#include "Errors.h"

vector<string> Errors::m_ErrorMsgs;

/**/
/*
Errors::InitErrorReporting()

NAME
	Errors::InitErrorReporting - Initializes the error reporting system.

SYNOPSIS
	void Errors::InitErrorReporting();

DESCRIPTION
	This function will clear the error messages and set the hasError flag to false.

RETURNS
	Void
*/
/**/
void Errors::InitErrorReporting() {
	m_ErrorMsgs.clear();
	hasError = false;
}

void Errors::RecordError(string a_emsg) {
	m_ErrorMsgs.push_back(a_emsg);
	hasError = true;
}

/**/
/*
Errors::DisplayErrors()
NAME
	Errors::DisplayErrors - Displays the errors that have been recorded.

SYNOPSIS
	void Errors::DisplayErrors();

DESCRIPTION
	This function will display the errors that have been recorded.

RETURNS
	Void
*/
/**/
void Errors::DisplayErrors() {
	if (hasError) {
		for (int i = 0; i < m_ErrorMsgs.size(); i++) {
			cout << m_ErrorMsgs[i] << endl;
		}
	}
	else {
		hasError = false;
		return;
	}
}

/**/
/*
Errors::NoErrors()
NAME
	Errors::NoErrors - Checks if there are any errors.

SYNOPSIS
	bool Errors::NoErrors();

DESCRIPTION
	This function will return true if there are no errors.

RETURNS
	True if there are no errors, false otherwise.

*/
/**/
bool Errors::NoErrors() {
	return m_ErrorMsgs.empty();
}