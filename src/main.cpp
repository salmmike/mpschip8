#include <iostream>
#include <cpu.h>
#include <screen.h>

int main(int argc, char** argv)
{
    Chip8CPU cpu;
    if (argc > 1) {
        cpu.readToMemory(argv[1]);
        cpu.run();
    }
    return 0;
}
