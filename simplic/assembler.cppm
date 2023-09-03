export module assembler;

#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include <tuple>
#include <algorithm>

namespace Simplic::Asm
{
    namespace Mnemonics
    {

        enum Type { Instruction, Register, Condition, ShiftOp, ShiftSel };

        const std::string INSTRUCTION[16] = {
            "MOV", "CNA", "LDR", "STR", "MVS", "SFT", "ADD", "SUB",
            "MUL", "LML", "DIV", "MOD", "AND", "ORR", "XOR", "NOR"
        };
        const std::string REGISTER[16] = {

            "ZR", "R1", "R2", "R3", "R4", "R5", // general purpose registers
            "R6", "R7", "R8", "R9", "RA", "RB",
            "SP", // Stack Pointer (points to the current top of the stack)
            "BR", // Buffer Register (for memory and jumps, whether using ADD or MOV)
            "LR", // Link Register (keep PC values to return to)
            "PC"  // Program counter
        };
        const std::string CONDITION[16] = {
            "ZS", "ZC", "CS", "CC", "NS", "NC", "VS", "VC",
            "SM", "HI", "LT", "GT", "LE", "GE", "FC", "AL"
        };
        const std::string SHIFTOP[4] = {
            "LSL", "LSR", "ASR", "ROR"
        };
        const std::string SHIFTSEL[2] = {
            "L", "R"
        };
    }

    // converts a mnemonic representation into a machine code in int
    // this uses an enum to simplify the function all a bit
    int MnemonicToInt(Mnemonics::Type type, std::string mnem)
    {
        const std::string* iter{}, * mnemBegin{}, * mnemEnd{};
        std::string typeName;

        switch (type)
        {
        case Mnemonics::Type::Instruction:
            mnemBegin = begin(Mnemonics::INSTRUCTION);
            mnemEnd = end(Mnemonics::INSTRUCTION);
            typeName = "Instruction";
            break;
        case Mnemonics::Type::Register:
            mnemBegin = begin(Mnemonics::REGISTER);
            mnemEnd = end(Mnemonics::REGISTER);
            typeName = "Register";
            break;
        case Mnemonics::Type::Condition:
            mnemBegin = begin(Mnemonics::CONDITION);
            mnemEnd = end(Mnemonics::CONDITION);
            typeName = "Condition";
            break;
        case Mnemonics::Type::ShiftOp:
            mnemBegin = begin(Mnemonics::SHIFTOP);
            mnemEnd = end(Mnemonics::SHIFTOP);
            typeName = "ShiftOp";
            break;
        case Mnemonics::Type::ShiftSel:
            mnemBegin = begin(Mnemonics::SHIFTSEL);
            mnemEnd = end(Mnemonics::SHIFTSEL);
            typeName = "ShiftSel";
            break;
        }

        std::transform(mnem.begin(), mnem.end(), mnem.begin(), ::toupper);
        iter = find(mnemBegin, mnemEnd, mnem);
        if (iter != mnemEnd)
        {
            return (int)distance(mnemBegin, iter);
        }
        else
        {
            std::cout << typeName << " unrecognized: " << mnem << std::endl;
            throw std::exception();
        }
    }

    //converts hex to int within specified sizeLimit
    int HexToInt(std::string hexstr, int sizeLimit)
    {
        std::transform(hexstr.begin(), hexstr.end(), hexstr.begin(), ::toupper);

        //check if hex
        size_t found = hexstr.find_first_not_of("0123456789ABCDEF", 0);
        if (found != std::string::npos)
        {
            std::cout << "Value not in hex: " << hexstr << std::endl;
            throw std::exception();
        }

        //convert to int
        unsigned int parsedValue;
        std::stringstream ss;
        ss << std::hex << hexstr;
        ss >> parsedValue;

        //check bit length
        int bitlength;
        int bitCounter = parsedValue;
        //keep counting bits and shifting right
        for (bitlength = 0; bitCounter != 0; bitlength++) bitCounter >>= 1;

        if (bitlength > sizeLimit)
        {
            std::cout << "Hex value too large for " << std::to_string(sizeLimit) << " bits: " << hexstr << std::endl;
            throw std::exception();
        }
        return parsedValue;
    }

    //Read individual line of asm
    uint16_t ReadAsmLine(std::string asmline)
    {
        if (asmline.find("//") == 0 || asmline == "") return -1;

        uint16_t mcCode = 0;
        std::stringstream sstream(asmline);
        std::string mnem;

        getline(sstream, mnem, ' ');
        int mcOpcode = MnemonicToInt(Mnemonics::Type::Instruction, mnem);
        mcCode += mcOpcode << 12; //opcode

        getline(sstream, mnem, ' ');
        mcCode += MnemonicToInt(Mnemonics::Type::Register, mnem) << 8; //rd

        //evaluating instr on a case-by-case basis
        switch (mcOpcode)
        {
        case 0: case 1: //for MOV and CNA, i.e. conditionals
            getline(sstream, mnem, ' ');
            mcCode += MnemonicToInt(Mnemonics::Type::Register, mnem) << 4; //rn
            getline(sstream, mnem, ' ');
            mcCode += MnemonicToInt(Mnemonics::Type::Condition, mnem); //cnd
            break;
        case 2: case 3:  //for LDR and STR, i.e. indexing
            getline(sstream, mnem, ' ');
            mcCode += MnemonicToInt(Mnemonics::Type::Register, mnem) << 4; //rn
            getline(sstream, mnem, ' ');
            mcCode += HexToInt(mnem, 4); //imm4 (indexing)
            break;
        case 4: //for MVS i.e. imm8
            getline(sstream, mnem, ' ');
            mcCode += HexToInt(mnem, 8); //imm8
            break;
        case 5: //for SFT is a bit complex
            getline(sstream, mnem, ' ');
            mcCode += MnemonicToInt(Mnemonics::Type::ShiftSel, mnem) << 6; //2bit sft operation
            getline(sstream, mnem, ' ');
            mcCode += MnemonicToInt(Mnemonics::Type::ShiftSel, mnem) << 5; //1bit select
            switch (MnemonicToInt(Mnemonics::Type::ShiftSel, mnem))
            {
            case 0:
                getline(sstream, mnem, ' ');
                mcCode += MnemonicToInt(Mnemonics::Type::Register, mnem); //rm
                break;

            case 1:
                getline(sstream, mnem, ' ');
                mcCode += HexToInt(mnem, 5); //imm5
                break;
            }
            break;
        default: //every other alu operations
            getline(sstream, mnem, ' ');
            mcCode += MnemonicToInt(Mnemonics::Type::Register, mnem) << 4; //rn
            getline(sstream, mnem, ' ');
            mcCode += MnemonicToInt(Mnemonics::Type::Register, mnem); //rm
            break;
        }

        return mcCode;
    }

    std::list<uint16_t> ReadAsm(std::list<std::string> asmlines)
    {
        //reading each lines separated by space ' '
        static std::list<uint16_t> mcodes;
        for (std::string line : asmlines)
        {
            uint16_t mcCode = ReadAsmLine(line);
            if ((int16_t)mcCode == -1) continue;
            mcodes.push_back(mcCode);
        }

        return mcodes;
    }

    void WriteToHexFile(std::string filename, std::list<uint16_t> mcodes)
    {
        std::ofstream File(filename);
        uint64_t address = 0;
        for (uint16_t instr : mcodes) {
            uint64_t mcCode = (uint64_t)0x02 << 48; //final machine code begins with 02 byte size
            mcCode += address << 32; //followed by address (increment by 2)
            mcCode += (uint64_t)instr << 8; //mode = 00 and then 2 data bytes
            uint8_t sum = 0; //checksum is the 2nd-comp of the sum of all bytes
            for (uint64_t mcBytes = mcCode >> 8; mcBytes != 0; mcBytes >>= 8) sum += (uint8_t)mcBytes;
            mcCode += (uint64_t)((uint8_t)~sum) + 1;
            address += 2; //increment the address for the next instruction
            File << ":0" << std::uppercase << std::hex << mcCode << std::endl;
        }

        File << ":00000001FF" << std::endl;
        File.close();
    }
}