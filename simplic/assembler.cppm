#include <iostream>;
#include <fstream>;
#include <sstream>;
#include <list>;
#include <tuple>;
#include <bitset>;

export module assembler;

namespace Simplic::Asm
{
    /// <summary> 
    /// Coverts opcode (in string) to binary instruction (in bitset)
    /// </summary>
    /// <param name='opcodeAsm'> the opcode in string </param>
    std::bitset<4> opcodeAsmToBin(std::string opcodeAsm);

    /// <summary> 
    /// Assemble a single asm line to a binary instruction.
    /// </summary>
    /// <param name='assembly'> a line of assembly </param>
    /// <returns> std::bitset<16> containing the instructions </returns>
    std::bitset<16> AssembleLineToBinary(std::string assembly);

    /// <summary> 
    /// Writes the a single line of binary instruction into the hex file
    /// </summary>
    /// <param name='file'> the ofstream of the file </param>
    /// <param name='instruction'> the instruction to write, in uint16 </param>
    void WriteBinaryToHexFile(std::ofstream& file, std::bitset<16> instruction);

    /// <summary> 
    /// Reads an from an ASM file and write to HEX file.
    /// </summary>
    /// <param name='asmfile'> the source asm file </param>
    /// <param name='hexfile'> the destination hex file </param>
    export void FileToFile(std::ifstream& asmfile, std::ofstream& hexfile)
    {
        std::string asmline;
        while (std::getline(asmfile, asmline))
        {
            // exclude comments and empty lines
            bool iscomment = asmline[0] != '/' || asmline[1] != '/';
            bool isempty = asmline.length() == 0;

            if (iscomment && !isempty)
            {
                std::cout << asmline << std::endl;

                std::bitset<16> binInstruction = AssembleLineToBinary(asmline);
                WriteBinaryToHexFile(hexfile, binInstruction);
            }
        }
    }
    
    std::bitset<4> opcodeAsmToBin(std::string opcodeAsm)
    {
        if (opcodeAsm == "MOV") return std::bitset<4>(0b0000);
        else if (opcodeAsm == "CNA") return std::bitset<4>(0b0001);
        else if (opcodeAsm == "LDR") return std::bitset<4>(0b0010);
        else if (opcodeAsm == "STR") return std::bitset<4>(0b0011);
        else if (opcodeAsm == "MVS") return std::bitset<4>(0b0100);
        else if (opcodeAsm == "SFT") return std::bitset<4>(0b0101);
        else if (opcodeAsm == "ADD") return std::bitset<4>(0b0110);
        else if (opcodeAsm == "SUB") return std::bitset<4>(0b0111);
        else if (opcodeAsm == "MUL") return std::bitset<4>(0b1000);
        else if (opcodeAsm == "LML") return std::bitset<4>(0b1001);
        else if (opcodeAsm == "DIV") return std::bitset<4>(0b1010);
        else if (opcodeAsm == "MOD") return std::bitset<4>(0b1011);
        else if (opcodeAsm == "AND") return std::bitset<4>(0b1100);
        else if (opcodeAsm == "ORR") return std::bitset<4>(0b1101);
        else if (opcodeAsm == "XOR") return std::bitset<4>(0b1110);
        else if (opcodeAsm == "NOR") return std::bitset<4>(0b1111);
        else
        {
            std::cout << "Cannot convert asm to binInstruction: " << opcodeAsm << std::endl;
            throw std::exception();
        }
    }

    std::bitset<16> AssembleLineToBinary(std::string assembly)
    {
        // the assembler works on capitalized letter, so convert it
        for (char& c : assembly) c = std::toupper(c);

        // let's start converting
        std::bitset<16> instruction;

        // I'll need to read this assembly with a space delimiter, i.e. use stringstream
        std::stringstream asmstream(assembly);

        // firstly, retrieve opcode and convert that to binary
        std::string opcodeAsm;
        std::getline(asmstream, opcodeAsm, ' ');
        std::bitset<4> opcodeBin = opcodeAsmToBin(opcodeAsm);

        // put the opcode bin into instruction
        for (int i = 0; i < 4; ++i) {
            instruction[15-i] = opcodeBin[i];
        }

        std::cout << instruction << std::endl;

        return instruction;
    }
    
    void WriteBinaryToHexFile(std::ofstream& file, std::bitset<16> instruction)
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
        fileHexcode += (uint64_t)instruction.to_ulong() << 8;

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
}