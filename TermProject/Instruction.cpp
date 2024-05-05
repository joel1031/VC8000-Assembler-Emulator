#include "stdafx.h"
#include "Errors.h"
#include "Instruction.h"

/**/
/*
Instruction::InstructionType Instruction::ParseInstruction(string a_line)

NAME
	Instruction::ParseInstruction - Parse the instruction.

SYNOPSIS
	Instruction::InstructionType Instruction::ParseInstruction(string a_line);
		a_line -> The line of code to be parsed.

DESCRIPTION
		This function will parse the instruction and determine the type of instruction.

RETURN
		Returns the type of instruction.
*/
/**/
Instruction::InstructionType Instruction::ParseInstruction(string a_line)
{
	// Record the original statement.  This will be needed in the sceond pass.
	m_instruction = a_line;

	// Delete any comment from the line.
	DeleteComment(a_line);

	// Record label, opcode, and operands.  Up to you to deal with formatting errors.
	bool isFormatError = RecordFields(a_line);

	// Check if this is a comment.
	if (m_Label.empty() && m_OpCode.empty())
	{
		return InstructionType::ST_Comment;
	}
	// Return the instruction type.  This has to be handled in the code.
	return m_type;
}

/**/
/*

Instruction::RecordFields(const string& a_line)

NAME
	Instruction::RecordFields - Record the fields of the instruction.

SYNOPSIS
	bool Instruction::RecordFields(const string& a_line);
		a_line -> The line of code to be parsed.

DESCRIPTION
	This function calls ParseLineIntoFields to parse the line into fields.
	It then records the fields of the instruction and determines the type of instruction.

RETURN
	Returns true if parsing the line into fields was successful, false otherwise.
*/
/**/
bool Instruction::RecordFields(const string& a_line)
{
	// Get the fields that make up the instruction.
	bool isFormatError = !ParseLineIntoFields(a_line, m_Label, m_OpCode, m_Operand1, m_Operand2);

	// if code was a comment, there is nothing to do.
	if (m_OpCode.empty() && m_Label.empty()) return isFormatError;

	// Record whether the operands are numeric and their value if they are.
	m_IsNumericOperand1 = isStrNumber(m_Operand1);
	if (m_IsNumericOperand1) m_Operand1NumericValue = stoi(m_Operand1);

	m_IsNumericOperand2 = isStrNumber(m_Operand2);
	if (m_IsNumericOperand2) m_Operand2NumericValue = stoi(m_Operand2);

	// For the sake of comparing, convert the op code to upper case.
	for (char& c : m_OpCode)
	{
		c = toupper(c);
	}
	// I leave to the class the tasks:
	// - Determine and record the instruction type from the op code.
	// - Recording the numberic Op code for machine lanuage equivalents.

	if (m_Label.empty() && m_OpCode.empty()) {
		m_type = InstructionType::ST_Comment;
	}
	else if (m_OpCode == "ORG" || m_OpCode == "DC" || m_OpCode == "DS") {
		m_type = InstructionType::ST_AssemblerInstr;
	}
	else if (m_OpCode == "END") {
		m_type = InstructionType::ST_End;
	}
	else {
		bool found = false;

		for (auto it = m_OpCodeMap.begin(); it != m_OpCodeMap.end(); it++) {
			if (m_OpCode == it->first) {
				m_NumOpCode = it->second;
				found = true;
				m_type = InstructionType::ST_MachineLanguage;
				break;
			}
		}

		if (!found) {
			m_type = InstructionType::ST_Error;
			Errors::RecordError("Error: Invalid op code: " + m_OpCode);
			Errors::DisplayErrors();
		}
	}

	return isFormatError;
}

/**/
/*

Instruction::ParseLineIntoFields()

NAME
	Instruction::ParseLineIntoFields - Parse the line into fields.

	SYNOPSIS
	bool Instruction::ParseLineIntoFields(string a_line, string& a_label, string& a_OpCode,
	string& a_Operand1, string& a_Operand2);
	a_line -> The line of code to be parsed.
	a_label -> The label of the instruction.
	a_OpCode -> The op code of the instruction.
	a_Operand1 -> The first operand of the instruction.
	a_Operand2 -> The second operand of the instruction.

DESCRIPTION
	This function will parse the line into fields and return the label, op code, and operands.

RETURN
	Returns true if there is no extra data, false otherwise.
*/
/**/
bool Instruction::ParseLineIntoFields(string a_line, string& a_label, string& a_OpCode,
	string& a_Operand1, string& a_Operand2)
{
	// Initialize the statrment elements to empty strings.
	a_label = a_OpCode = a_Operand1 = a_Operand2 = "";

	// If this is an empty string, return indicating that it is OK.
	if (a_line.empty()) return false;

	// Get rid of any commas from the line.
	replace(a_line.begin(), a_line.end(), ',', ' ');

	// Get the elements of the line.  That is the label, op code, operand1, and operand2.
	string endStr;
	istringstream ins(a_line);
	if (a_line[0] == ' ' || a_line[0] == '\t')
	{
		a_label = "";
		ins >> a_OpCode >> a_Operand1 >> a_Operand2 >> endStr;
	}
	else
	{
		ins >> a_label >> a_OpCode >> a_Operand1 >> a_Operand2 >> endStr;
	}
	// If there is extra data, return true.
	return !endStr.empty();
}

/**/
/*

Instruction::isStrNumber()

NAME
	Instruction::isStrNumber - Determine if a string is a number.

SYNOPSIS
	bool Instruction::isStrNumber(const string& a_str);
	a_str -> The string to be checked.

DESCRIPTION
	This function will determine if a string is a number.

RETURN
	Returns true if the string is a number, false otherwise.
*/
/**/
bool Instruction::isStrNumber(const string& a_str)
{
	if (a_str.empty()) return false;

	// If there is a - or a plus, make sure there are more characters.
	int ichar = 0;
	if (a_str[0] == '-' || a_str[0] == '+')
	{
		ichar++;
		if (a_str.length() < 2) return false;
	}
	// Make sure that the remaining characters are all digits
	for (; ichar < a_str.length(); ichar++)
	{
		if (!isdigit(a_str[ichar])) return false;
	}
	return true;
}

/**/
/*
Instruction::LocationNextInstruction()

NAME
	LocationNextInstruction - Determine the location of the next instruction.

SYNOPSIS
	int LocationNextInstruction(int a_loc);
	a-loc -> The location of the current instruction.

DESCRIPTION
	This function will determine the location of the next instruction.
	It uses the op code to determine the location of the next instruction.

RETURN
	The location of the next instruction.
*/
/**/
int Instruction::LocationNextInstruction(int a_loc) {
	if (m_OpCode == "ORG" || m_OpCode == "DS") {
		if (m_IsNumericOperand1)
			return a_loc + m_Operand1NumericValue;
	}
	return a_loc + 1;
}

/**/
/*
Instruction::DeleteComment()

NAME
	getAssembly - Get the machine translation for an instruction.

SYNOPSIS
	string Instruction::getAssembly(SymbolTable& const m_symtab);
	m_symtab -> The symbol table which contains the labels and their locations.

DESCRIPTION
	This function converts one line of source code into machine language and returns it as a string.

RETURN
	String containing the machine language equivalent of the instruction.

*/
/**/

string Instruction::getMachine(SymbolTable& const m_symtab) {
	string machineCode = "";
	string opcode = "";
	string reg1 = "9";
	string reg2 = "9";
	string address = "";

	opcode = to_string(m_NumOpCode);
	opcode = opcode.insert(0, 2 - opcode.length(), '0');

	int tempLoc = 0;

	//all operands need to have an operand1 except for the halt instruction
	if (m_Operand1.empty() && m_OpCode != "HALT") {
		Errors::RecordError("Operand 1 is missing");
		Errors::DisplayErrors();
		return opcode + "???????";
	}

	//halt instruction cant have any operands
	if (m_OpCode == "HALT") {
		if (!m_Operand1.empty()) {
			Errors::RecordError("Halt instruction should not have any operands");
			Errors::DisplayErrors();
			return opcode + "0000000";
		}
		return opcode + "0000000";
	}

	//opcodes with two registers
	if (m_NumOpCode >= 7 && m_NumOpCode <= 10) {
		//check if operand1 is not numeric
		if (!m_IsNumericOperand1 && !m_IsNumericOperand2) {
			Errors::RecordError("Error: Both operands must be numeric");
			Errors::DisplayErrors();
			return opcode + "?" + "?" + "00000";
		}

		//check if operand1 is numeric
		if (!m_IsNumericOperand1) {
			Errors::RecordError("Error: Operand 1 must be numeric");
			Errors::DisplayErrors();
			return opcode + "?" + m_Operand2 + "00000";
		}

		//check if operand2 is numeric
		if (!m_IsNumericOperand2) {
			Errors::RecordError("Error: Operand 2 must be numeric");
			Errors::DisplayErrors();
			return opcode + m_Operand1 + "?" + "00000";
		}

		//check if the regirster value is valid
		reg1 = (stoi(m_Operand1) > 9 || stoi(m_Operand1) < 0) ? "?" : m_Operand1;
		reg2 = (stoi(m_Operand2) > 9 || stoi(m_Operand2) < 0) ? "?" : m_Operand2;
		if (reg1 == "?" || reg2 == "?") {
			Errors::RecordError("Error: Register value must be between 0 and 9");
			Errors::DisplayErrors();
		}
		return opcode + reg1 + reg2 + "00000";
	}

	//for opcodes 1 - 10, operand1 must be numeric
	if (m_NumOpCode >= 1 && m_NumOpCode <= 10) {
		if (!m_IsNumericOperand1) {
			Errors::RecordError("Error: Operand 1 must be numeric");
			Errors::DisplayErrors();
			return opcode + "?" + "0000000";
		}
	}

	//for numerical operand1, operand2 should be symbolic
	//if no operand2, then operand1 should be symbolic
	if (m_IsNumericOperand1) {
		if (!m_symtab.LookupSymbol(m_Operand2, tempLoc)) {
			Errors::RecordError("Error: Undefined symbol: " + m_Operand2);
			Errors::DisplayErrors();
			return opcode + m_Operand1 + "??????";
		}

		if (tempLoc == m_symtab.multiplyDefinedSymbol) {
			Errors::RecordError("Error: Multiply defined symbol: " + m_Label);
			Errors::DisplayErrors();
			return opcode + m_Operand1 + "??????";
		}

		//check if regirster value is valid.
		reg1 = (m_Operand1NumericValue < 0 || m_Operand1NumericValue > 9) ? "?" : m_Operand1;
		if (reg1 == "?") {
			Errors::RecordError("Error: Register value must be between 0 and 9");
			Errors::DisplayErrors();
		}

		address = to_string(tempLoc);
		address = address.insert(0, 6 - address.length(), '0');
		machineCode = opcode + reg1 + address;
	}
	else {
		if (!m_Operand2.empty()) {
			Errors::RecordError("Error: Operand 2 must be empty for symbolic operand 1");
			Errors::DisplayErrors();
		}

		if (!m_symtab.LookupSymbol(m_Operand1, tempLoc)) {
			Errors::RecordError("Error: Undefined symbol: " + m_Operand1);
			Errors::DisplayErrors();
			return opcode + "9" + "??????";
		}

		if (tempLoc == m_symtab.multiplyDefinedSymbol) {
			Errors::RecordError("Error: Multiply defined symbol: " + m_Operand1);
			Errors::DisplayErrors();
			return opcode + "9" + "??????";
		}

		address = to_string(tempLoc);
		address = address.insert(0, 6 - address.length(), '0');
		machineCode = to_string(m_NumOpCode) + "9" + address;
	}

	return machineCode;
}

/**/
/*
Instruction::getAssembly()

NAME
	getMachine - Get the machine translation for an instruction.

SYNOPSIS
	string Instruction::getMachine(SymbolTable& const m_symtab);
	m_symtab -> The symbol table which contains the labels and their locations.

DESCRIPTION
	This function converts one line of source code into machine language and returns it as a string.

RETURN
	String containing the machine language equivalent of the instruction.

*/
/**/
string Instruction::getAssembly(SymbolTable& const m_symtab) {
	string machineCode = "";

	if (m_Operand1.empty()) {
		Errors::RecordError("Operand 1 is missing for " + m_OpCode);
		Errors::DisplayErrors();
	}
	else if (!m_IsNumericOperand1) {
		Errors::RecordError("Operand 1 must be numeric for " + m_OpCode);
		Errors::DisplayErrors();
	}

	if (!m_Operand2.empty()) {
		Errors::RecordError("Operand 2 must be empty for " + m_OpCode);
		Errors::DisplayErrors();
	}

	//org should not have any operands
	if (m_OpCode == "ORG") {
		if (!m_Label.empty()) {
			Errors::RecordError("ORG should not have a label.");
			Errors::DisplayErrors();
		}
	}
	else if (m_Label.empty()) {
		Errors::RecordError("Error: Label mssing for " + m_OpCode);
		Errors::DisplayErrors();
	}

	if (m_OpCode == "DC") {
		machineCode = m_Operand1;

		if (m_Operand1NumericValue > 10'000) {
			Errors::RecordError("ERROR: Constant too large for DC (max size=10,000)!");
			Errors::DisplayErrors();
			machineCode = "??????";
		}

		machineCode = machineCode.insert(0, 9 - machineCode.length(), '0');
	}

	if (m_OpCode == "DS") {
		if (m_Operand1NumericValue > 10'000) {
			Errors::RecordError("ERROR: DS operand too large (max size=10,000)!");
			Errors::DisplayErrors();
		}
	}

	return machineCode;
}