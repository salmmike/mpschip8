#ifndef CHIP8_CPU_H
#define CHIP8_CPU_H

#include <vector>
#include <stdint.h>
#include <screen.h>
#include <string>
#include <chrono>

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


bool getByte(uint8_t N, uint16_t sprite);

class Chip8CPU
{
public:

    Chip8CPU();
    ~Chip8CPU() {};

    opcode fetch();
    void execute(opcode inst);

    void setMemory(std::vector<uint8_t> data);
    void setPC(size_t p);

    void run();
    void init();
    void step();

    uint8_t getMemory(size_t n) { return memory[n]; };

    void readToMemory(std::string filename);

private:
    std::vector<uint8_t> memory;
    std::vector<uint8_t> V;
    std::vector<uint16_t> stack;

    int16_t I {0};
    size_t pc {0};

    uint8_t delayTimer{0};
    uint8_t soundTimer{0};

    std::chrono::milliseconds prevTime;

    std::unique_ptr<Chip8Screen> screen;

    std::chrono::milliseconds currentTime();

    void OP0(opcode inst);
    void OP1(opcode inst);
    void OP2(opcode inst);
    void OP3(opcode inst);
    void OP4(opcode inst);
    void OP5(opcode inst);
    void OP6(opcode inst);
    void OP7(opcode inst);
    void OP8(opcode inst);
    void OP9(opcode inst);
    void OPA(opcode inst);
    void OPB(opcode inst);
    void OPC(opcode inst);
    void OPD(opcode inst);
    void OPE(opcode inst);
    void OPF(opcode inst);

    void display(opcode inst);
};

#endif