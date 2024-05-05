#include "stdafx.h"
#include "Emulator.h"

/**/
/*
Emulator::insertMemory()
NAME

	Emulator::insertMemory - inserts translated code into memory.

SYNOPSIS
	 bool Emulator::insertMemory(int a_location, long long a_contents);
	 a_location -> The location in memory to insert the translated code.
	 a_contents -> The translated code to insert into memory.

DESCRIPTION
	Checks if the location is within the memory size.
	If it is, the translated code is inserted into memory.

RETURN
	Returns true if the translated code was inserted into memory.
*/
/**/
bool emulator::insertMemory(int a_location, long long a_contents)
{
	if (a_location >= 0 && a_location < 1'000'000) {
		m_memory[a_location] = a_contents;
		return true;
	}
	else {
		cout << "Error: Memory location out of bounds." << endl;
		return false;
	}
}

/**/
/*
meulator::runProgra,()
NAME

	Emulator::runProgram - runs the emulator.

SYNOPSIS
	 bool Emulator::runProgram();

DESCRIPTION
	The function runs the emulator.

RETURN
	Returns true if the emulator ran successfully.

*/
/**/
bool emulator::runProgram() {
	cout << "Emulator:\n\n";
	int contents = 0;
	int opcode = 0;
	int reg = 0;
	int reg2 = 0;
	int address = 0;
	int input = 0;

	for (int loc = 0; loc < MEMSZ; loc++) {
		contents = m_memory[loc];

		if (contents != 0) {
			opcode = contents / 10'000'000;
			reg = (contents / 1'000'000) % 10;
			reg2 = (contents / 100'000) % 10;
			address = contents % 1'000'000;

			switch (opcode) {
			case 1:
				m_reg[reg] += m_memory[address];
				break;

			case 2:
				m_reg[reg] -= m_memory[address];
				break;

			case 3:
				m_reg[reg] *= m_memory[address];
				break;

			case 4:
				m_reg[reg] /= m_memory[address];
				break;

			case 5:
				m_reg[reg] = m_memory[address];
				break;

			case 6:
				m_memory[address] = m_reg[reg];
				break;

			case 7:
				m_reg[reg] += m_reg[reg2];
				break;

			case 8:
				m_reg[reg] -= m_reg[reg2];
				break;

			case 9:
				m_reg[reg] *= m_reg[reg2];
				break;

			case 10:
				m_reg[reg] /= m_reg[reg2];
				break;

			case 11:
				cout << "?";
				cin >> input;
				input %= 1000000;
				m_memory[address] = input;
				break;

			case 12:
				cout << m_memory[address] << endl;
				break;

			case 13:
				loc = address;
				break;

			case 14:
				if (m_reg[reg] < 0)
				{
					loc = address - 1;
				}
				break;

			case 15:
				if (m_reg[reg] == 0) {
					loc = address - 1;
				}
				break;

			case 16:
				if (m_reg[reg] > 0) {
					loc = address - 1;
				}
				break;

			case 17:
				cout << "End of emulation\n";
				exit(1);
				break;

			default:
				cout << "Error." << endl;
				return false;
			}
		}
	}
	return false;
}