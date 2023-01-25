#include <cpu.h>
#include <iostream>
#include <algorithm>
#include <cassert>
#include <unistd.h>
#include <fstream>
#include <filesystem>
#include <stdlib.h>
#include <time.h>
#include <ios>

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

    std::cout << "pc: " << pc << ", op: " << std::hex << oc.opcode << "\n";

    pc += 2;
    return oc;
}

void Chip8CPU::execute(opcode inst)
{
    auto op = inst.op();
    switch (op)
    {
        case 0x0:
            OP0(inst);
            break;
        case 0x1:
            OP1(inst);
            break;
        case 0x2:
            OP2(inst);
            break;
        case 0x3:
            OP3(inst);
            break;
        case 0x4:
            OP4(inst);
            break;
        case 0x5:
            OP5(inst);
            break;
        case 0x6:
            OP6(inst);
            break;
        case 0x7:
            OP7(inst);
            break;
        case 0x8:
            OP8(inst);
            break;
        case 0x9:
            OP9(inst);
            break;
        case 0xA:
            OPA(inst);
            break;
        case 0xB:
            OPB(inst);
            break;
        case 0xC:
            OPC(inst);
            break;
        case 0xD:
            OPD(inst);
            break;
        case 0xE:
            OPE(inst);
            break;
        case 0xF:
            OPF(inst);
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
    init();
    while (true) {
        step();
        usleep(1000 * 100);
    }
}

void Chip8CPU::init()
{
    srandom(time(NULL));
    screen->create();
}

void Chip8CPU::step()
{
    opcode op = fetch();
    std::cout << "Sleep\n";
    execute(op);
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
            }
        }
    }
    screen->draw();
}

void Chip8CPU::OP0(opcode inst)
{
    switch (inst.NNN())
    {
    case 0x0E0:
        clear(inst);
        break;
    case 0x0EE:
        pc = stack.back();
        stack.pop_back();
    default:
        break;
    }
}

void Chip8CPU::OP1(opcode inst)
{
    jump(inst);
}

void Chip8CPU::OP2(opcode inst)
{
    stack.push_back(pc);
    pc = inst.NNN();
}

void Chip8CPU::OP3(opcode inst)
{
    if (V[inst.X()] == inst.NN()) {
        pc += 2;
    }
}

void Chip8CPU::OP4(opcode inst)
{
    if (V[inst.X()] != inst.NN()) {
        pc += 2;
    }
}

void Chip8CPU::OP5(opcode inst)
{
    if (V[inst.X()] == V[inst.Y()]) {
        pc += 2;
    }
}

void Chip8CPU::OP6(opcode inst)
{
    setVX(inst);
}

void Chip8CPU::OP7(opcode inst)
{
    addVX(inst);
}

void Chip8CPU::OP8(opcode inst)
{
    switch (inst.N())
    {
    case 0:
        V[inst.X()] = V[inst.Y()];
        break;
    case 1:
        V[inst.X()] |= V[inst.Y()];
        break;
    case 2:
        V[inst.X()] &= V[inst.Y()];
        break;
    case 3:
        V[inst.X()] ^= V[inst.Y()];
        break;
    case 4:
        V[0xF] = (((uint16_t) V[inst.X()] + (uint16_t) V[inst.Y()]) > 255) ? 1 : 0;
        V[inst.X()] += V[inst.Y()];
        break;
    case 5:
        V[inst.X()] -= V[inst.Y()];
        break;

    case 6:
        V[0xF] = (V[inst.Y()] & 0b00000001) ? 1 : 0;
        V[inst.X()] = (V[inst.Y()] >> 1);
        break;
    case 7:
        V[inst.X()] = V[inst.Y()] - V[inst.X()];
        break;
    case 0xE:
        V[0xF] = (V[inst.Y()] & 0b10000000) ? 1 : 0;
        V[inst.X()] = (V[inst.Y()] << 1);
        break;

    default:
        break;
    }
}

void Chip8CPU::OP9(opcode inst)
{
    if (V[inst.X()] != V[inst.Y()]) {
        pc += 2;
    }
}

void Chip8CPU::OPA(opcode inst)
{
    setI(inst);
}

void Chip8CPU::OPB(opcode inst)
{
    pc = inst.NNN() + V[0];
}

void Chip8CPU::OPC(opcode inst)
{
    V[inst.X()] = (inst.NN() & static_cast<uint8_t>(random()));
}

void Chip8CPU::OPD(opcode inst)
{
    display(inst);
}

void Chip8CPU::OPE(opcode inst)
{
    switch (inst.NN())
    {
    case 0x9E:
        if (screen->checkKey(inst.X())) {
            pc += 2;
        }
        break;
    case 0xA1:
        if (!screen->checkKey(inst.X())) {
            pc += 2;
        }
        break;
    default:
        break;
    }
}

void Chip8CPU::OPF(opcode inst)
{
    switch (inst.NN())
    {
    case 0x07:
        V[inst.X()] = delayTimer;
        break;
    case 0x15:
        delayTimer = V[inst.X()];
        break;
    case 0x18:
        soundTimer = V[inst.X()];
        break;
    case 0x1E:
        I += V[inst.X()];
        break;
    case 0x0A:
        // Wait any key;
        break;
    case 0x29:
        I = 0x050 + inst.X() * 5;
        break;
    case 0x33:
        memory[I + 2] = V[inst.X()] % 10;
        memory[I + 1] = (V[inst.X()] / 10) % 10;
        memory[I] = (V[inst.X()] / 100) % 10;
        break;
    case 0x55:
        for (uint8_t i = 0; i <= inst.X(); ++i) {
            memory[I + i] = V[i];
        }
        break;
    case 0x65:
        for (uint8_t i = 0; i <= inst.X(); ++i) {
            V[i] = memory[I + i];
        }
        break;
    default:
        break;
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
