#include <iostream>
#include <cpu.h>
#include <screen.h>

int main()
{
    Chip8CPU cpu;
    //cpu.readToMemory("/home/mike/Code/chip8/IBM_logo.ch8");
    cpu.readToMemory("/home/mike/Code/chip8/test_opcode.ch8");
    cpu.run();

    return 0;
}
