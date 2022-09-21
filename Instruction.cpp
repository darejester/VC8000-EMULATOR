#include "stdafx.h"

#include "Instruction.h"

/**/
/*
Instruction::ParseInstruction()

NAME

        Instruction::ParseInstruction()

SYNOPSIS

        Instruction::InstructionType Instruction::ParseInstruction(string a_line)
        a_line      --> the line to be parsed

DESCRIPTION

        This function reads the instruction to be parsed

RETURNS

        Returns instruction type for the current line

*/
/**/
// parse an instruction and return its type.  Yes, I want a call by value.
Instruction::InstructionType Instruction::ParseInstruction(string a_line)
{
    // Record the original statement.  This will be needed in the second pass.
    m_instruction = a_line;

    // Delete any comment from the line.
    DeleteComment( a_line );

    // Record label, opcode, and operands.  Up to you to deal with formatting errors.
    bool isFormatError = RecordFields( a_line );
    
    // Check if this is a comment.
    if (m_Label.empty() && m_OpCode.empty())
    {
        return InstructionType::ST_Comment;
    }

    //check if end statement
    if (m_OpCode == "END")
    {
        return InstructionType::ST_End;
    }

    // figure out type of op_code
    if (find(m_machine.begin(), m_machine.end(), m_OpCode) != m_machine.end()) // check if machine language
    {
        // numeric representation of machine language op code
        m_NumOpCode = distance(m_machine.begin(), find(m_machine.begin(), m_machine.end(), m_OpCode)) + 1;
        return InstructionType::ST_MachineLanguage;
    }
    else if (find(m_assembly.begin(), m_assembly.end(), m_OpCode) != m_assembly.end()) // check if assembly language
    {
        return InstructionType::ST_AssemblerInstr;
    }
    else // if not machine or assembly
    {
        return InstructionType::ST_Error;
    }

    // Return the instruction type.
    //return m_type;
}

/**/
/*
Instruction::RecordFields()

NAME

        Instruction::RecordFields()

SYNOPSIS

        bool Instruction::RecordFields( const string &a_line )
        a_line      --> the line to be parsed

DESCRIPTION

        This function reads the instruction line and checks if the format is correct

RETURNS

        Returns true if the format of an instruction line is correct, false if not


*/
/**/
// Record the fields that make up the instructions.
bool Instruction::RecordFields( const string &a_line )
{
    // Get the fields that make up the instruction.
    bool isFormatError = !ParseLineIntoFields( a_line, m_Label, m_OpCode, m_Operand1, m_Operand2);

    // if code was a comment, there is nothing to do.
    if (m_OpCode.empty() && m_Label.empty()) return isFormatError;

    // Record whether the operands are numeric and their value if they are.
    m_IsNumericOperand1 = IsStrNumber(m_Operand1);
    if (m_IsNumericOperand1) m_Operand1NumericValue = stoi(m_Operand1);

    m_IsNumericOperand2 = IsStrNumber(m_Operand2);
    if (m_IsNumericOperand2) m_Operand1NumericValue = stoi(m_Operand2);

    // For the sake of comparing, convert the op code to upper case.
    for (char& c : m_OpCode)
    {
        c = toupper(c);
    }
    // I leave to the class the tasks:
    // - Determine and record the instruction type from the op code.
    // - Recording the numberic Op code for machine lanuage equivalents.
    return true;
}


/**/
/*
Instruction::ParseLineIntoFields()

NAME

        Instruction::ParseLineIntoFields()

SYNOPSIS

        bool Instruction::ParseLineIntoFields(string a_line, string& a_label, string& a_OpCode, string& a_Operand1, string& a_Operand2)
        a_line      --> the line to be parsed
        a_label     --> records a label if any
        a_OpCode    --> records the opcode of the instuction
        a_Operand1  --> records the first operand of the instruction if any
        a_Operand2  --> records the second operand of the instruction if any

DESCRIPTION

        This function records the different parts of the instruction into their respective containers

RETURNS

        Returns true if the instruction does not have any extra/unnecessary information


*/
/**/
// Parse the intruction into fields. sets label, opcode, and operands 1&2
bool Instruction::ParseLineIntoFields(string a_line, string& a_label, string& a_OpCode, string& a_Operand1, string& a_Operand2)
{
    // Get rid of any commas from the line.
    replace(a_line.begin(), a_line.end(), ',', ' ');

    // Get the elements of the line.  That is the label, op code, operand1, and operand2.
    string endStr;
    a_label = a_OpCode = a_Operand1 = a_Operand2 = "";
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
    // If there is extra data, return false.
    return endStr.empty() ? true : false;
}

/**/
/*
Instruction::LocationNextInstruction()

NAME

        Instruction::LocationNextInstruction()

SYNOPSIS

        int Instruction::LocationNextInstruction(int a_loc)
        a_loc      --> the location of the next instruction line

DESCRIPTION

        This function finds the location of the next instruction

RETURNS

        Returns an int that represents the location of the next instruction


*/
/**/
// find location of next instruction
int Instruction::LocationNextInstruction(int a_loc) {
    //cout << "Must implement: LocationNextInstruction( )" << endl;

    if (m_OpCode == "ORG")
    {
        return a_loc += m_Operand1NumericValue;
    }
    else if (m_OpCode == "DC")
    {
        return a_loc += 1;
    }
    else if (m_OpCode == "DS")
    {
        return a_loc += m_Operand1NumericValue;
    }
    else
    {
        return a_loc += 1;
    }

    
}


/**/
/*
Instruction::IsStrNumber()

NAME

        Instruction::IsStrNumber()

SYNOPSIS

        bool Instruction::IsStrNumber(const string& a_str)
        a_str      --> the string that will be checked if it has a number

DESCRIPTION

        This function checks if a string contains a number

RETURNS

        Returns true if string has a number, false if not


*/
/**/
bool Instruction::IsStrNumber(const string& a_str)
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
