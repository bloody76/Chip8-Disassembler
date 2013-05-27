/*
** Vincent LATROUITE <vincentlat.bloody@gmail.com>
** Started on  <jeu. 23/mai/2013 at 13:29> <latrou_v>
** Last Update <jeu. 23/mai/2013 at 13:29> <latrou_v>
*/
#include <map>
#include <cstring>
#include <iostream>
#include <sstream>
#include <fstream>

typedef unsigned short uint16;
typedef unsigned char uint8;

enum class Type
{
    SUB,
    ADR
};

std::map<uint16, Type>  functions;
uint8*                  mem;
std::ifstream::pos_type size;

void parse(std::ifstream& rom)
{
    uint8   op1 = 0;
    uint8   op2 = 0;
    uint16  op  = 0;

    size = rom.tellg();
    mem = new uint8[size];
    rom.seekg(0, std::ios::beg);
    rom.read((char*)mem, size);
    rom.close();

    for (int i = 0; i < size; i += 2)
    {
        op1 = mem[i];
        if (i + 1 < size)
            op2 = mem[i + 1];
        else
        {
            std::cerr << "Invalid opcode, exiting." << std::endl;
            exit(1);
        }

        op = (op1 << 8) | op2;
        switch ((op & 0xF000) >> 12)
        {
            case 1:
                functions[op & 0x0FFF] = Type::ADR;
                break;
            case 2:
                functions[op & 0x0FFF] = Type::SUB;
                break;
        }
    }
}

void pretty_print()
{
    for (int i = 0; i < size; i += 2)
    {
        auto it = functions.find(i + 0x200);
        if (it != functions.end())
        {
            std::cout << "\t";
            if (it->first == 0x200)
                std::cout << "start";
            else
            {
                if (it->second == Type::SUB)
                    std::cout << "sub_";
                else
                    std::cout << "adr_";
                std::cout << it->first;
            }
            std::cout  << ":" << std::endl;
        }
        uint16 op = (mem[i] << 8) | mem[i + 1];
        {
            std::cout << "0x" << std::hex << op << std::dec << "|\t\t";
            switch ((op & 0xF000) >> 12)
            {
                case 0:
                    if ((op & 0x0FFF) == 0xE0)
                        std::cout << "CLS";
                    else if ((op & 0x0FFF) == 0xEE)
                        std::cout << "RET";
                    else
                        std::cout << "GOTO adr:" << (op & 0x0FFF);
                    break;
                case 1:
                    std::cout << "JP " << (op & 0x0FFF);
                    break;
                case 2:
                    std::cout << "CALL sub_" << (op & 0x0FFF);
                    break;
                case 3:
                    std::cout << "SE V" << ((op & 0x0F00) >> 8) << ", " << (op & 0x00FF);
                    break;
                case 4:
                    std::cout << "SNE V" << ((op & 0x0F00) >> 8) << ", " << (op & 0x00FF);
                    break;
                case 5:
                    std::cout << "SE V" << ((op & 0x0F00) >> 8) << ", V" << ((op & 0x00F0) >> 4);
                    break;
                case 6:
                    std::cout << "LD V" << ((op & 0x0F00) >> 8) << ", " << (op & 0x00FF);
                    break;
                case 7:
                    std::cout << "ADD V" << ((op & 0x0F00) >> 8) << ", " << (op & 0x00FF);
                    break;
                case 8:
                    switch (op & 0x000F)
                    {
                        case 0:
                            std::cout << "LD V" << ((op & 0x00F0) >> 4) << ", V" << ((op & 0x0F00) >> 8);
                            break;
                        case 1:
                            std::cout << "OR V" << ((op & 0x0F00) >> 8) << ", V" << ((op & 0x00F0) >> 4);
                            break;
                        case 2:
                            std::cout << "AND V" << ((op & 0x0F00) >> 8) << ", V" << ((op & 0x00F0) >> 4);
                            break;
                        case 3:
                            std::cout << "XOR V" << ((op & 0x0F00) >> 8) << ", V" << ((op & 0x00F0) >> 4);
                            break;
                        case 4:
                            std::cout << "ADD V" << ((op & 0x0F00) >> 8) << ", V" << ((op & 0x00F0) >> 4);
                            break;
                        case 5:
                            std::cout << "SUB V" << ((op & 0x0F00) >> 8) << ", V" << ((op & 0x00F0) >> 4);
                            break;
                        case 6:
                            std::cout << "SHR V" << ((op & 0x0F00) >> 8);
                            break;
                        case 7:
                            std::cout << "SUBN V" << ((op & 0x0F00) >> 8) << ", V" << ((op & 0x00F0) >> 4);
                            break;
                        case 0xE:
                            std::cout << "SHL V" << ((op & 0x0F00) >> 8);
                            break;
                    }
                    break;
                case 9:
                    std::cout << "SNE V" << ((op & 0x0F00) >> 8) << ", V" << ((op & 0x00F0) >> 4);
                    break;
                case 0xA:
                    std::cout << "LD I, " << (op & 0x0FFF);
                    break;
                case 0xB:
                    std::cout << "JP V0, " << (op & 0x0FFF);
                    break;
                case 0xC:
                    std::cout << "RND V" << ((op & 0x0F00) >> 8) << ", " << (op & 0x00FF);
                    break;
                case 0xD:
                    std::cout << "DRW V" << ((op & 0x0F00) >> 8) << ", V" << ((op & 0x00F0) >> 4) << ", " << (op & 0x00FF);
                    break;
                case 0xE:
                    switch (op & 0x00FF)
                    {
                        case 0x9E:
                            std::cout << "SKP V" << ((op & 0x0F00) >> 8);
                            break;
                        case 0xA1:
                            std::cout << "SKNP V" << ((op & 0x0F00) >> 8);
                            break;
                    }
                    break;
                case 0xF:
                    switch (op & 0x00FF)
                    {
                        case 0x07:
                            std::cout << "LD V" << ((op & 0x0F00) >> 8) << ", DT";
                            break;
                        case 0x0A:
                            std::cout << "LD V" << ((op & 0x0F00) >> 8) << ", K";
                            break;
                        case 0x15:
                            std::cout << "LD DT, V" << ((op & 0x0F00) >> 8);
                            break;
                        case 0x18:
                            std::cout << "LD ST, V" << ((op & 0x0F00) >> 8);
                            break;
                        case 0x1E:
                            std::cout << "ADD I, V" << ((op & 0x0F00) >> 8);
                            break;
                        case 0x29:
                            std::cout << "LD F, V" << ((op & 0x0F00) >> 8);
                            break;
                        case 0x33:
                            std::cout << "LD B, V" << ((op & 0x0F00) >> 8);
                            break;
                        case 0x55:
                            std::cout << "LD [I], V" << ((op & 0x0F00) >> 8);
                            break;
                        case 0x65:
                            std::cout << "LD V" << ((op & 0x0F00) >> 8) << ", [i]";
                            break;
                    }
                    break;
                default:
                    std::cout << op;
                    break;
            }
            std::cout << std::endl;
        }
    }
}

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cerr << "First argument must be a filename." << std::endl;
        return 1;
    }
    std::string file(argv[1]);

    functions[0x200] = Type::SUB;
    std::ifstream rom(file, std::ios::ate);
    if (!rom.is_open())
    {
        std::cerr << "Fill with a valid filename." << std::endl;
        return 1;
    }
    parse(rom);
    pretty_print();
    delete[] mem;
    return 0;
}
