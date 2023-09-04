#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include <map>

export module assembler;

namespace Simplic::Asm
{
    /// <summary> 
    /// Reads opcode mnemonic from string stream and converts to binary
    /// </summary>
    /// <param name='asmstream'> the string stream of the current asm line </param>
    /// <returns> a binary representation of the opcode </returns>
    uint16_t OpcodeToBin (std::stringstream& asmstream)
    {
        const std::map<std::string, uint16_t> OPCODES =
        {
            { "MOV", 0x0000},
            { "CNA", 0x1000},
            { "LDR", 0x2000},
            { "STR", 0x3000},
            { "MVS", 0x4000},
            { "SFT", 0x5000},
            { "ADD", 0x6000},
            { "SUB", 0x7000},
            { "MUL", 0x8000},
            { "LML", 0x9000},
            { "DIV", 0xA000},
            { "MOD", 0xB000},
            { "AND", 0xC000},
            { "ORR", 0xD000},
            { "XOR", 0xE000},
            { "NOR", 0xF000},
        };
        
        std::string opcodeAsm;
        std::getline(asmstream, opcodeAsm, ' ');
        if (OPCODES.find(opcodeAsm) != OPCODES.end())
        {
            return OPCODES.at(opcodeAsm);
        }
        else
        {
            std::cout << "ASM ERROR: Opcode not recognized: " << opcodeAsm << std::endl;
            throw std::exception();
            return 0;
        }
    }
    
    /// <summary> 
    /// Reads register mnemonic from string stream and converts to binary
    /// </summary>
    /// <param name='asmstream'> the string stream of the current asm line </param>
    /// <returns> a binary representation of the register </returns>
    uint16_t RegisterToBin(std::stringstream& asmstream)
    {
        const std::map<std::string, uint16_t> REGISTERS =
        {
            { "ZR", 0x0},
            { "R1", 0x1},
            { "R2", 0x2},
            { "R3", 0x3},
            { "R4", 0x4},
            { "R5", 0x5},
            { "R6", 0x6},
            { "R7", 0x7},
            { "R8", 0x8},
            { "R9", 0x9},
            { "RA", 0xA},
            { "RB", 0xB},
            { "SP", 0xC},
            { "BR", 0xD},
            { "LR", 0xE},
            { "PC", 0xF},
        };
    
        std::string regAsm;
        std::getline(asmstream, regAsm, ' ');
        if (REGISTERS.find(regAsm) != REGISTERS.end())
        {
            return REGISTERS.at(regAsm);
        }
        else
        {
            std::cout << "ASM ERROR: Register not recognized: " << regAsm << std::endl;
            throw std::exception();
            return 0;
        }
    }

    /// <summary> 
    /// Reads condition mnemonic from string stream and converts to binary
    /// </summary>
    /// <param name='asmstream'> the string stream of the current asm line </param>
    /// <returns> a binary representation of the condition </returns>
    uint16_t ConditionToBin(std::stringstream& asmstream)
    {
        const std::map<std::string, uint16_t> CONDITIONS =
        {
            { "ZS", 0x0},
            { "ZC", 0x1},
            { "CS", 0x2},
            { "CC", 0x3},
            { "NS", 0x4},
            { "NC", 0x5},
            { "VS", 0x6},
            { "VC", 0x7},
            { "SM", 0x8},
            { "HI", 0x9},
            { "LT", 0xA},
            { "GT", 0xB},
            { "LE", 0xC},
            { "GE", 0xD},
            { "FC", 0xE},
            { "AL", 0xF},
        };

        std::string cndAsm;
        std::getline(asmstream, cndAsm, ' ');
        if (CONDITIONS.find(cndAsm) != CONDITIONS.end())
        {
            return CONDITIONS.at(cndAsm);
        }
        else
        {
            std::cout << "ASM ERROR: Register not recognized: " << cndAsm << std::endl;
            throw std::exception();
            return 0;
        }
    }
    
    /// <summary> 
    /// Reads shift operation mnemonic from string stream and converts to binary
    /// </summary>
    /// <param name='asmstream'> the string stream of the current asm line </param>
    /// <returns> a binary representation of the shift operation </returns>
    uint16_t ShiftToBin(std::stringstream& asmstream)
    {
        uint16_t shiftBin = 0;
        std::string shiftOpAsm;
        std::getline(asmstream, shiftOpAsm, ' ');

        // shiftop determines what shifing to do i.e. LSL LSR ASR or ROR
        if (shiftOpAsm == "LSL") shiftBin = (uint16_t)0b00 << 6;
        else if (shiftOpAsm == "LSR") shiftBin = (uint16_t)0b01 << 6;
        else if (shiftOpAsm == "ASR") shiftBin = (uint16_t)0b10 << 6;
        else if (shiftOpAsm == "ROR") shiftBin = (uint16_t)0b11 << 6;
        else
        {
            std::cerr << "ASM ERROR: shiftop not recognized: " << shiftOpAsm << std::endl;
            throw std::exception();
        }

        // shiftsel determines where the shifing value comes from
        // whether from immediate value or register value
        std::string shiftselAsm;
        std::getline(asmstream, shiftselAsm, ' ');
        if (shiftselAsm == "I") shiftBin += (uint16_t)0b00 << 5;
        else if (shiftselAsm == "R") shiftBin += (uint16_t)0b01 << 5;
        else
        {
            std::cerr << "ASM ERROR: shift select not recognized: " << shiftselAsm << std::endl;
            throw std::exception();
        }

        return shiftBin;
    }

    /// <summary> 
    /// Reads immediate hex from string stream and converts to binary
    /// </summary>
    /// <param name='asmstream'> the string stream of the current asm line </param>
    /// <returns> a binary representation of the immediate </returns>
    uint16_t HexstringToBin(std::stringstream& asmstream, uint16_t sizeRestriction)
    {
        std::string hexString;
        std::getline(asmstream, hexString, ' ');
        uint16_t value = 0;
        try
        {
            value = std::stoi(hexString, nullptr, 16);
        }
        catch (const std::exception&)
        {
            std::cout << "ASM ERROR: Cannot parse the hex value: " << hexString << std::endl;
            throw std::exception();
        }

        if (value > (0b1 << sizeRestriction))
        {
            std::cout << "ASM ERROR: hex value too big" << hexString << std::endl;
            throw std::exception();
        }

        return value;
    }

    /// <summary> 
    /// Reads an from an ASM file and write to HEX file.
    /// </summary>
    /// <param name='asmfile'> the source asm file </param>
    /// <param name='hexfile'> the destination hex file </param>
    /// <returns> a list of binary instructions </returns>
    export std::list<uint16_t> AsmfileToBinary(std::ifstream& asmfile)
    {
        // list of the result binary instructions
        std::list <uint16_t> binInstructions;
        // a string buffer to store the current line of assembly
        std::string asmline;
        // start reading line by line
        while (std::getline(asmfile, asmline))
        {
            // exclude comments and empty lines
            bool iscomment = asmline[0] != '/' || asmline[1] != '/';
            bool isempty = asmline.length() == 0;
            if (iscomment && !isempty)
            {
                // the assembler works on capitalized letter, so convert it to upper
                for (char& c : asmline) c = std::toupper(c);

                // I'll need to read this assembly with a space delimiter, i.e. use stringstream
                std::stringstream asmstream(asmline);

                // let's start the conversion
                // firstly, retrieve opcode and convert that to binary
                uint16_t currentInstruction = OpcodeToBin(asmstream);
                // also, keep track of this opcode to help parse the rest of ASM
                uint16_t opcodeBin = currentInstruction;

                // next, retrieve destination register i.e. Rd
                currentInstruction += RegisterToBin(asmstream) << 8;

                // now, parse the rest of the instructions
                // note that the format is dependent on what the opcode is at hand
                switch (opcodeBin)
                {
                    // for MOV and CNA instruction, i.e. with conditionals
                    // format: OPCODE Rd Rn CND
                    case 0x0000: case 0x1000:
                        currentInstruction += RegisterToBin(asmstream) << 4; // Rn
                        currentInstruction += ConditionToBin(asmstream); // CND
                        break;
                    // for LDR and STR instruction, i.e. with imm4 indexing
                    // format: OPCODE Rd Rn Imm4
                    case 0x2000: case 0x3000:
                        currentInstruction += RegisterToBin(asmstream) << 4; // Rn
                        currentInstruction += HexstringToBin(asmstream, 4); // Imm4 Indexing
                        break;
                    // for MVS instruction, i.e. with imm8 
                    // format: OPCODE Rd Imm8
                    case 0x4000:
                        currentInstruction += HexstringToBin(asmstream, 8); // Imm8 value
                        break;
                    // for SFT instruction, which is a little complex
                    // format: OPCODE Rd SHIFTOP SHIFTSEL
                    case 0x5000:
                        currentInstruction += ShiftToBin(asmstream);
                        // now determine whether the shiftsel bit is set or not
                        if ((currentInstruction & 1 << 5) >> 5 == 0) // not set; use immediate Imm5
                        {
                            currentInstruction += HexstringToBin(asmstream, 5); 
                        }
                        else // shiftsel bit set; use register value
                        {
                            currentInstruction += RegisterToBin(asmstream);
                        }

                        break;
                    //every other alu operations
                    default: 
                        currentInstruction += RegisterToBin(asmstream) << 4; // Rn
                        currentInstruction += RegisterToBin(asmstream); // Rm
                        break;
                }


                binInstructions.push_back(currentInstruction);
            }
        }

        return binInstructions;
    }
    
    /// <summary> 
    /// Writes binary instructions to HEX file.
    /// </summary>
    /// <param name='file'> the destination hex file </param>
    /// <param name='instructions'> the list of binary instructions </param>
    export void BinaryToHexfile(std::string filename, std::list<uint16_t> instructions)
    {
        std::ofstream file(filename);
        for (uint16_t instruction : instructions)
        {
            // the actual hex code to be written to file in .hex format
            uint64_t fileHexcode;

            // a static int to keep track of address (increments address every time an instr is written)
            static uint64_t address = 0;

            // firstly, the hex code begins with 02 byte size
            fileHexcode = (uint64_t)0x02 << 48;

            // followed by the address
            fileHexcode += address << 32;

            // mode = 00 and then put the instruction in as 2 data bytes
            fileHexcode += (uint64_t)instruction << 8;

            //checksum is the 2nd-comp of the sum of all bytes
            uint8_t sum = 0;
            for (uint64_t mcBytes = fileHexcode >> 8; mcBytes != 0; mcBytes >>= 8)
                sum += (uint8_t)mcBytes;

            // then write the checksum to hex code
            fileHexcode += (uint64_t)((uint8_t)~sum) + 1;

            // then writes the hex code to file
            file << ":0" << std::uppercase << std::hex << fileHexcode << std::endl;

            // lastly, increment the address
            address += 2;
        }

        file << ":00000001FF" << std::endl;
        file.close();
    }
}