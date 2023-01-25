#include <cpu.h>
#include <iostream>
#include <algorithm>
#include <cassert>
#include <unistd.h>
#include <fstream>
#include <filesystem>

constexpr std::array<uint8_t, 5*16> makeFontVec()
{
    return {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };
}

Chip8CPU::Chip8CPU()
{
    memory.resize(4096);
    pc = 0x200;
    V.resize(0x10);

    auto fontVec = makeFontVec();
    std::copy(fontVec.begin(), fontVec.end(), memory.begin() + 0x050);

    screen = std::make_unique<Chip8Screen>();
}

opcode Chip8CPU::fetch()
{
    uint8_t a = memory[pc];
    uint8_t b = memory[pc + 1];

    opcode oc;
    oc.opcode = (a << 8) | b;

    std::cout << "pc: " << pc << ", op: " << oc.opcode << "\n";

    pc += 2;
    return oc;
}

void Chip8CPU::decode(opcode inst)
{
    auto op = inst.op();
    switch (op)
    {
        case 0x0:
            clear(inst);
            break;
        case 0x1:
            jump(inst);
            break;
        case 0x6:
            setVX(inst);
            break;
        case 0x7:
            addVX(inst);
            break;
        case 0xA:
            setI(inst);
            break;
        case 0xD:
            display(inst);
            break;
        default:
            std::cout << op << " not implemented\n";
            break;
    }
}

void Chip8CPU::setMemory(std::vector<uint8_t> data)
{
    memory = data;
}

void Chip8CPU::setPC(size_t p)
{
    pc = p;
}

void Chip8CPU::run()
{
    screen->create();
    while (true) {
        opcode op = fetch();
        std::cout << "Sleep\n";
        usleep(1000 * 100);

        decode(op);
    }
}

void Chip8CPU::readToMemory(std::string filename)
{
    std::ifstream infile;
    infile.open(filename, std::ifstream::in | std::ifstream::binary);

    std::cout << filename << "exists: " << std::filesystem::exists(filename) << "\n";
    std::cout << std::filesystem::canonical(filename) << "\n";

    infile.seekg(0, std::ios::end);
    size_t length = infile.tellg();
    infile.seekg(0, std::ios::beg);

    infile.read(reinterpret_cast<char*>(&memory[0x200]), length);

    infile.close();
}

void Chip8CPU::display(opcode inst)
{
    auto x = V[inst.X()] % 64;
    auto y = V[inst.Y()] % 32;
    V[0xF] = 0;

    for (uint8_t i = 0; i < inst.N(); ++i) {
        uint8_t sprite = memory[I + i];
        for (uint8_t N = 0; N < 8; ++N) {
            if (getByte(N, sprite)) {
                V[0xF] |= screen->flip(x + N, y + i);
                screen->draw();
            }
        }
    }
}

void Chip8CPU::setVX(opcode inst)
{
    V[inst.X()] = inst.NN();
}

void Chip8CPU::addVX(opcode inst)
{
    V[inst.X()] += inst.NN();
}

void Chip8CPU::setI(opcode inst)
{
    I = inst.NNN();
}

void Chip8CPU::jump(opcode inst)
{
    pc = inst.NNN();
}

void Chip8CPU::clear(opcode inst)
{
    screen->clear();
}

bool getByte(uint8_t N, uint16_t sprite)
{
    assert(N < 8);
    uint16_t mask = 1 << (7 - N);
    return (sprite & mask);
}
