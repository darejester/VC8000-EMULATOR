//
//      Implementation of the Assembler class.
//
#include "stdafx.h"
#include "Assembler.h"
#include "Errors.h"
#include <iomanip>

// Constructor for the assembler.  Note: we are passing argc and argv to the file access constructor.
// See main program.  
Assembler::Assembler( int argc, char *argv[] )
: m_facc( argc, argv )
{
    // Nothing else to do here at this point.
} 

// Destructor 
Assembler::~Assembler( )
{
}

/**/
/*
Assembler::PassI()

NAME

        Assembler::PassI()

SYNOPSIS

        void Assembler::PassI()

DESCRIPTION

        This function runs the first pass in the assembler.
        It attempts to read all instructions and get its instruction type.
        If the instruction is a label/symbol, it will record the location and symbol in the symbol table.

RETURNS

        Returns nothing.

*/
/**/
// Pass I establishes the location of the labels.  You will write better function comments according to the coding standards.
void Assembler::PassI( ) 
{
    int loc = 0;        // Tracks the location of the instructions to be generated.

    // Successively process each line of source code.
    for( ; ; ) {

        // Read the next line from the source file.
        string line; 
        if( ! m_facc.GetNextLine( line ) ) {

            // If there are no more lines, we are missing an end statement.
            // We will let this error be reported by Pass II.
            return;
        }
        // Parse the line and get the instruction type.
        Instruction::InstructionType st =  m_inst.ParseInstruction( line );

        // If this is an end statement, there is nothing left to do in pass I.
        // Pass II will determine if the end is the last statement and report an error if it isn't.
        if( st == Instruction::InstructionType::ST_End ) return;

        // Labels can only be on machine language and assembler language
        // instructions.  So, skip comments.
        if( st == Instruction::InstructionType::ST_Comment )  
        {
        	continue;
	    }
        
        // If the instruction has a label, record it and its location in the
        // symbol table.
        if( m_inst.IsLabel( ) ) {

            m_symtab.AddSymbol( m_inst.GetLabel( ), loc );
        }

        // Compute the location of the next instruction.
        loc = m_inst.LocationNextInstruction( loc );
    }
}

/**/
/*
Assembler::PassII()

NAME

        Assembler::PassII()

SYNOPSIS

        void Assembler::PassII()

DESCRIPTION

        This function runs the second pass in the assembler.
        It attempts to read all instructions and translate them

RETURNS

        Returns nothing.


*/
/**/
// Pass II translates instructions
bool err = false;
void Assembler::PassII() {
    //cout << "Must implement: PassII( )" << endl;
    m_facc.rewind();
    Errors::InitErrorReporting; // initialize error vector
    int loc = 0;        // Tracks the location of the instructions to be generated.
    string content = ""; // holds content of current line

    // print header
    cout << setw(3) << "LOCATION" << setw(1) << " " << setw(9) << "CONTENTS" << setw(6) << " " << "ORIGINAL STATEMENTS" << endl;

    // Successively process each line of source code.
    for (; ; ) {

        // Read the next line from the source file.
        string line;
        if (!m_facc.GetNextLine(line)) {

            // If there are no more lines, we are missing an end statement.
            // We will let this error be reported by Pass II.
            Errors::RecordError("No END statement found.");
            Errors::DisplayErrors();
            err = true;
            return;
        }
        // Parse the line and get the instruction type.
        Instruction::InstructionType st = m_inst.ParseInstruction(line);

        // If this is an end statement, there is nothing left to do in pass I.
        // Pass II will determine if the end is the last statement and report an error if it isn't.
        if (st == Instruction::InstructionType::ST_End)
        {
            cout << setw(24) << " " << line << endl;
            return;
        }

        // Labels can only be on machine language and assembler language
        // instructions.  So, skip comments.
        if (st == Instruction::InstructionType::ST_Comment)
        {
            cout << setw(24) << " " << line << endl;
            continue;
        }

        // Handle the case where there is an error.
        if (st == Instruction::InstructionType::ST_Error)
        {
            Errors::RecordError("Instruction has an Error.");
            err = true;
        }


        // register to use
        string pholder = "9";

        // if instruction is machine language or assembler language
        if (st == Instruction::InstructionType::ST_MachineLanguage)
        {
            // add opcode section to content
            while (content.length() < (2 - (m_inst.GetOpCodeNum()).length()))
            {
                content += "0";
            }
            content += m_inst.GetOpCodeNum();


            //add register section to content
            if (m_inst.GetOpCodeNum() == "17") // if opcode is HALT
            {
                content += pholder;
                while (content.length() < (10 - pholder.length()))
                {
                    content += "0";
                }
            }
            else if (m_inst.GetOpCodeNum() == "11") // if opcode is READ
            {
                content += pholder;
            }
            else
            {
                //add register section to content
                content += m_inst.GetOperand1();
            }
            

            // add location section to content
            if (m_inst.GetOpCodeNum() == "11") // if read
            {
                if (m_symtab.LookupSymbol(m_inst.GetOperand1(), loc) == 0)
                {
                    Errors::RecordError("Symbol is not in symbol table.");
                    err = true;
                }
                else // add location section to content
                {
                    string temp = m_inst.GetOperand1();

                    //add address section to content
                    while (content.length() < (9 - (m_symtab.LookupLocation(temp).length())))
                    {
                        content += "0";
                    }
                    content += m_symtab.LookupLocation(temp);

                }

            }
            else if(m_inst.GetOpCodeNum() != "17") // if not HALT
            {
                // check if symbol is in symbol table
                if (m_symtab.LookupSymbol(m_inst.GetOperand2(), loc) == 0)
                {
                    Errors::RecordError("Symbol is not in symbol table.");
                    err = true;
                }
                else // add location section to content
                {
                    string temp = m_inst.GetOperand2();

                    //add address section to content
                    while (content.length() < (9 - (m_symtab.LookupLocation(temp).length())))
                    {
                        content += "0";
                    }
                    content += m_symtab.LookupLocation(temp);
                }
            }

 
        }
        else if (st == Instruction::InstructionType::ST_AssemblerInstr)
        {
            if (m_inst.GetOpCode() == "ORG")
            {
                /*cout << setw(24) << " " << line << endl;
                continue;*/
            }
            else if(m_inst.GetOpCode() == "DS")
            {
                //cout << setw(24) << " " << line << endl;
            }
            else if (m_inst.GetOpCode() == "END")
            {
                cout << setw(24) << " " << line << endl;
                continue;
            }
            else
            {
                while (content.length() < (9 - m_inst.GetOperand1().length()))
                {
                    content += "0";
                }
                content += m_inst.GetOperand1();
            }

        }

        // print translation and original statement
        cout << setw(3) << loc << setw(6) << " " << setw(9) << content << setw(6) << " " << line << endl;

        // if org, do nothing
        if (m_inst.GetOpCode() == "ORG" || m_inst.GetOpCode() == "DS")
        {

        } 
        else if ( !(m_emul.insertMemory( loc, (long long)(stoi(content)) )) )// convert content into int//insert content to memory
        {
            Errors::RecordError("Not enough Memory");
            err = true;
        }
        
        // clear content for next line
        content = "";
        Errors::DisplayErrors();

        // Compute the location of the next instruction.
        loc = m_inst.LocationNextInstruction(loc);

        
    }
    
}


/**/
/*
Assembler::RunProgramInEmulator()

NAME

        Assembler::RunProgramInEmulator()

SYNOPSIS

        void Assembler::RunProgramInEmulator()

DESCRIPTION

        This function calls the runProgram() function of the emulator class to run the emulator

RETURNS

        Returns nothing.


*/
/**/
void Assembler::RunProgramInEmulator()
{
    //cout << "Must implement: RunProgramInEmulator( )" << endl;
    if (err == true) { cout << "Instruction has an error. Cannot run emulator"; exit(0); }
    m_emul.runProgram();
}