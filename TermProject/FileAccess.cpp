//
//  Implementation of file access class.
//
#include "stdafx.h"
#include "FileAccess.h"

/**/
/*
FileAccess::FileAccess()

NAME
	FileAccess::FileAccess - constructor for the FileAccess class.

SYNOPSIS
	FileAccess::FileAccess(int argc, char* argv[]);
	argc	-> The number of arguments passed to the program.
	argv	-> An array of strings containing the arguments passed to the program.

DESCRIPTION
	This function will open the file passed to the program.
*/
/**/
FileAccess::FileAccess(int argc, char* argv[])
{
	// Check that there is exactly one run time parameter.
	if (argc != 2) {
		cerr << "Usage: Assem <FileName>" << endl;
		exit(1);
	}
	// Open the file.  One might question if this is the best place to open the file.
	// One might also question whether we need a file access class.
	m_sfile.open(argv[1], ios::in);

	// If the open failed, report the error and terminate.
	if (!m_sfile) {
		cerr << "Source file could not be opened, assembler terminated."
			<< endl;
		exit(1);
	}
}

/**/
/*

FileAccess::~FileAccess()

NAME
	FileAccess::~FileAccess - destructor for the FileAccess class.

SYNOPSIS
	FileAccess::~FileAccess();

DESCRIPTION
	This function will close the file passed to the program.

RETURN
		None
*/
/**/
FileAccess::~FileAccess()
{
	// Not that necessary in that the file will be closed when the program terminates, but good form.
	m_sfile.close();
}

/**/
/*

FileAccess::GetNextLine()

NAME
	FileAccess::GetNextLine - gets the next line from the file.

SYNOPSIS
	bool FileAccess::GetNextLine(string& a_line);
	a_line	-> The line to be read from the file.

DESCRIPTION
		This function will get the next line from the file.

RETURN
		Returns true if the next line was read successfully.
*/
/**/
bool FileAccess::GetNextLine(string& a_line)
{
	// If there is no more data, return false.
	if (m_sfile.eof()) {
		return false;
	}
	getline(m_sfile, a_line);

	// Return indicating success.
	return true;
}

/**/
/*

FileAccess::rewind()

NAME
	FileAccess::rewind - rewinds the file.

SYNOPSIS
	void FileAccess::rewind();

DESCRIPTION
	This function will rewind the file, meaning it will go back to the beginning of the file.

RETURN
		None
*/
/**/
void FileAccess::rewind()
{
	// Clean all file flags and go back to the beginning of the file.
	m_sfile.clear();
	m_sfile.seekg(0, ios::beg);
}