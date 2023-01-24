#ifndef CHIP8_CPU_H
#define CHIP8_CPU_H

#include <vector>
#include <stdint.h>

struct opcode
{
    uint16_t opcode;
    uint8_t op() {
        return ((opcode & 0xF000) >> 12);
    };
    uint8_t X() {
        return ((opcode & 0x0F00) >> 8);
    };
    uint8_t Y() {
        return ((opcode & 0x00F0) >> 4);
    };
    uint8_t N() {
        return (opcode & 0x000F);
    };
    uint8_t NN() {
        return (opcode & 0x00FF);
    };
    uint16_t NNN() {
        return opcode & 0x0FFF;
    };
};

class Chip8CPU
{
public:

    Chip8CPU();
    ~Chip8CPU() {};

    opcode fetch();
    void decode(opcode inst);
    //void? execute();

    void setMemory(std::vector<uint8_t> data);
    void setPC(size_t p);

    uint8_t getMemory(size_t n) { return memory[n]; };

private:
    std::vector<uint8_t> memory;
    std::vector<int16_t> varReg;
    std::vector<int16_t> stack;

    int16_t I {0};
    size_t pc {0};
    uint8_t delayTimer{0};
    uint8_t soundTimer{0};

};

#endif