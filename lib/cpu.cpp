#include <cpu.h>
#include <iostream>

void test()
{
    std::cout << "test\n";
}

opcode Chip8CPU::fetch()
{
    uint8_t a = memory[pc];
    uint8_t b = memory[pc + 1];
    pc += 2;

    opcode oc;
    oc.opcode = (a << 8) | b;

    return oc;
}

void Chip8CPU::decode(opcode inst)
{
}

void Chip8CPU::setMemory(std::vector<uint8_t> data)
{
    memory = data;
}

void Chip8CPU::setPC(size_t p)
{
    pc = p;
}
