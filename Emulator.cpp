#include "stdafx.h"
#include "Emulator.h"


/**/
/*
emulator::insertMemory()

NAME

		emulator::insertMemory()

SYNOPSIS

		bool emulator::insertMemory(int a_location, long long a_contents)
		a_location			--> Memory location in which we insert the contents
		a_contents			--> contents to be inserted into memory location

DESCRIPTION

		This function inserts a_contents into the m_memory vector at the location specified by a_location

RETURNS

		Returns true if contents are successfully inserted, returns false if not.


*/
/**/
bool emulator::insertMemory(int a_location, long long a_contents)
{
	if (a_location >= MEMSZ)
	{
		return false;
	}
	else
	{
		m_memory[a_location] = a_contents;
		return true;
	}
}


/**/
/*
emulator::runProgram()

NAME

		emulator::runProgram()

SYNOPSIS

		bool emulator::runProgram()

DESCRIPTION

		This function attempts to simulate the VC8000 computer by executing the operation for the respective operation codes

RETURNS

		Returns true if the instruction has run successfully, returns false if not.


*/
/**/
bool emulator::runProgram()
{
	//cout << "Must implement: runProgram()" << endl;
	string temp = "";
	string op_code = "";
	string reg = "";
	string address = "";
	int input = 0;
	// if content length is < 9, opcode is a single digit
	// if content start with "00", it does not have an opcode

	for (int loc = 100; loc < MEMSZ; loc++)
	{
		address.clear();
		op_code.clear();
		temp = (to_string)(m_memory[loc]); // convert contents into string

		// get opcode, register, address
		if (temp.length() < 4) // if no opcode
		{
			op_code = "0";
		}
		else if (temp.length() < 9) // if opcode is < 10
		{
			op_code = temp[0];
			reg = temp[1];
			address.push_back(temp[2]);
			address.push_back(temp[3]);
			address.push_back(temp[4]);
			address.push_back(temp[5]);
			address.push_back(temp[6]);
			address.push_back(temp[7]);
			//cout << "opcode is < 10" << endl;
		}
		else // if opcode is > 10
		{
			op_code.push_back(temp[0]);
			op_code.push_back(temp[1]);
			reg = temp[2];
			address.push_back(temp[3]);
			address.push_back(temp[4]);
			address.push_back(temp[5]);
			address.push_back(temp[6]);
			address.push_back(temp[7]);
			address.push_back(temp[8]);

		}

		switch (stoi(op_code)) // converts the content's opcode into an int
		{
			// add
			case 1:
				m_register[stoi(reg)] = (m_register[stoi(reg)] + m_memory[stoi(address)]);
				break;

			// sub
			case 2:
				m_register[stoi(reg)] = (m_register[stoi(reg)] - m_memory[stoi(address)]);
				break;

			// mult
			case 3:
				m_register[stoi(reg)] = (m_register[stoi(reg)] * m_memory[stoi(address)]);
				break;

			//div
			case 4:
				m_register[stoi(reg)] = (m_register[stoi(reg)] / m_memory[stoi(address)]);
				break;

			// load
			case 5:
				m_register[stoi(reg)] = m_memory[stoi(address)];
				break;

			// store
			case 6:
				m_memory[stoi(address)] = m_register[stoi(reg)];
				break;

			// addr
			case 7:
				m_register[stoi(reg)] = (m_register[stoi(reg)] + m_register[stoi(address)]);
				break;

			// subr
			case 8:
				m_register[stoi(reg)] = (m_register[stoi(reg)] - m_register[stoi(address)]);
				break;

			// multr
			case 9:
				m_register[stoi(reg)] = (m_register[stoi(reg)] * m_register[stoi(address)]);
				break;

			// divr
			case 10:
				m_register[stoi(reg)] = (m_register[stoi(reg)] / m_register[stoi(address)]);
				break;

			// read
			case 11:
				cout << "? ";
				cin >> input;
				m_memory[stoi(address)] = input;
				break;

			// write
			case 12:
				cout << m_memory[stoi(address)] << endl;
				break;

			// B
			case 13:
				loc = stoi(address) - 1;
				break;

			// BM
			case 14:
				if (m_register[stoi(reg)] < 0)
				{
					loc = stoi(address) - 1;
				}
				break;

			// BZ
			case 15:
				if (m_register[stoi(reg)] == 0)
				{
					loc = stoi(address) - 1;
				}
				break;

			// BP
			case 16:
				if (m_register[stoi(reg)] > 0)
				{
					loc = stoi(address)-1;
				}
				break;

			// HALT
			case 17:
				return true;
				break;
	
		}

	}


	return true;
}
