//
//      Implementation of the Assembler class.
//
#include "stdafx.h"
#include "Assembler.h"
#include "Errors.h"
/**/
/*

NAME
	Assembler - Constructor for the Assembler class.

SYNOPSIS
	Assembler::Assembler(int argc, char* argv[]);
	argc	-> The number of arguments passed to the program.
	argv	-> An array of strings containing the arguments passed to the program.

DESCRIPTION
	This function will initialize the file access object with the arguments passed to the program.

RETURNS
	None

AUTHOR
	Joel Acosta

DATE
	2:36pm 5/1/2024

*/
/**/
Assembler::Assembler(int argc, char* argv[])
	: m_facc(argc, argv)
{
	// Nothing else to do here at this point.
}

// Destructor for the Assembler class.
Assembler::~Assembler()
{
}

/**/
/*
Assembler::PassI()
NAME
	Assembler PassI() - Fills symbol table with labels and their locations.

SYNOPSIS
	void Assembler::PassI();

DESCRIPTION
	This function will fill the symbol table with the labels and their locations.

RETURNS
	void

*/
/**/
void Assembler::PassI()
{
	int loc = 0;
	// Tracks the location of the instructions to be generated.

	// Successively process each line of source code.
	for (; ; ) {
		// Read the next line from the source file.
		string line;
		if (!m_facc.GetNextLine(line)) {
			// If there are no more lines, we are missing an end statement.
			// We will let this error be reported by Pass II.
			return;
		}
		// Parse the line and get the instruction type.
		Instruction::InstructionType st = m_inst.ParseInstruction(line);

		// If this is an end statement, there is nothing left to do in pass I.
		// Pass II will determine if the end is the last statement and report an error if it isn't.
		if (st == Instruction::InstructionType::ST_End) return;

		// Labels can only be on machine language and assembler language
		// instructions.  So, skip comments.
		if (st == Instruction::InstructionType::ST_Comment)
		{
			continue;
		}
		// Handle the case where there is an error.

		// If the instruction has a label, record it and its location in the
		// symbol table.
		if (m_inst.isLabel()) {
			m_symtab.AddSymbol(m_inst.GetLabel(), loc);
		}
		// Compute the location of the next instruction.
		loc = m_inst.LocationNextInstruction(loc);
	}
}

/**/
/*
Assembler::PassII()
NAME
	Assembler::PassII - Translation for code and error checking.

SYNOPSIS
	void Assembler::PassII();

DESCRIPTION
	This function will translate the code from the source code file into machine code.
	It will also check for errors in the code.

RETURNS
	void
*/
/**/
void Assembler::PassII()
{
	cout << "Press Enter to continue...";
	cin.ignore();
	cout << "Translation of Program:" << endl;
	cout << left << "Location   " << "Contents   " << "Original Statement" << endl;
	// Rewind the file, Pass I needs to read the file.
	m_facc.rewind();

	// Initialize the location.
	int loc = 0;
	string conts = "";
	long long num_con = 0;

	// Successively process each line of source code.
	for (; ; ) {
		// Read the next line from the source file.
		string line;
		if (!m_facc.GetNextLine(line)) {
			Errors::RecordError("Error: No end statement");
			Errors::DisplayErrors();
			return;
		}
		// Parse the line and get the instruction type.
		Instruction::InstructionType st = m_inst.ParseInstruction(line);

		// If this is an end statement, there is nothing left to do in pass I.
		// Pass II will determine if the end is the last statement and report an error if it isn't.
		if (st == Instruction::InstructionType::ST_End) {
			cout << "\t\t" << line << endl;
			return;
		}

		// Skip comments.
		if (st == Instruction::InstructionType::ST_Comment)
		{
			cout << "\t\t\t" << line << "\n"; //print the comment line
			continue;
		}

		if (m_inst.isLabel()) {
			if (!m_inst.isValidLabel()) {
				Errors::RecordError("Error: Invalid label: " + m_inst.GetLabel());
				Errors::DisplayErrors();
			}
		}

		if (st == Instruction::InstructionType::ST_MachineLanguage) {
			conts = m_inst.getMachine(m_symtab);
		}
		else if (st == Instruction::InstructionType::ST_AssemblerInstr) {
			conts = m_inst.getAssembly(m_symtab);
		}

		if (!conts.empty()) {
			num_con = stoll(conts);
			m_emul.insertMemory(loc, num_con);
		}

		cout << left << setw(10) << loc << setw(10) << conts << setw(10) << line << endl;
		loc = m_inst.LocationNextInstruction(loc);

		if (loc < 0 || loc > 999'999) {
			Errors::RecordError("Error: Location out of bounds");
			Errors::DisplayErrors();
			return;
		}
	}
	RunProgramInEmulator();
	cout << endl;
}

/**/
/*
Assembler::RunProgramInEmulator()
NAME
	RunProgramInEmulator - Runs the emulator.

SYNOPSIS
	void Assembler::RunProgramInEmulator();

DESCRIPTION
	This function will run the program in the emulator if there are no errors.

RETURNS
	void
*/
/**/
void Assembler::RunProgramInEmulator()
{
	cout << "Press Enter to continue...";
	cin.ignore();

	if (!Errors::NoErrors()) {
		cout << "Emulator could not run due to errors in the program." << endl;
	}
	else {
		cout << "Results from emulating program:" << endl;
		m_emul.runProgram();
	}
	cout << endl;
	cout << "End of emulation." << endl;
}