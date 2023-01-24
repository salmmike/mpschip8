#include <gtest/gtest.h>
#include <iostream>
#include <cassert>
#include <cpu.h>

/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * Author: Mike Salmela
 */

#define NAME Chip8Test

void testOpcode(opcode op, std::vector<uint16_t> data)
{
    ASSERT_TRUE(op.op() == data[0]) << "Opcode: " << (int) op.op()
                              << " Expected: " << (int) data[0];

    ASSERT_TRUE(op.X() == data[1]) << "X: " << (int) op.X()
                              << " Expected: " << (int) data[1];

    ASSERT_TRUE(op.Y() == data[2]) << "Y: " << (int) op.Y()
                              << " Expected: " << (int) data[2];

    ASSERT_TRUE(op.N() == data[3]) << "N: " << (int) op.N()
                              << " Expected: " << (int) data[3];

    ASSERT_TRUE(op.NN() == data[4]) << "NN: " << (int) op.NN()
                              << " Expected: " << (int) data[4];

    ASSERT_TRUE(op.NNN() == data[5]) << "NNN: " << (int) op.NN()
                              << " Expected: " << (int) data[5];
}

TEST(NAME, test_fetch)
{
    std::vector<uint8_t> mem {0xDE, 0xAD, 0xBE, 0xEF};

    Chip8CPU cpu;
    cpu.setPC(0);
    cpu.setMemory(mem);

    opcode op = cpu.fetch();

    ASSERT_TRUE(op.opcode == 0xDEAD);
    std::vector<uint16_t> testdata {0xD, 0xE, 0xA, 0xD, 0xAD, 0xEAD};
    testOpcode(op, testdata);

    op = cpu.fetch();
    testdata = std::vector<uint16_t> {0xB, 0xE, 0xE, 0xF, 0xEF, 0xEEF};
    testOpcode(op, testdata);
}

TEST(NAME, check_font)
{
    Chip8CPU cpu;
    ASSERT_TRUE(cpu.getMemory(0x050) == 0xF0) << "Value: " << cpu.getMemory(0x050)
                                              << "Expected " << 0x050;
    ASSERT_TRUE(cpu.getMemory(0x09F) == 0x80) << "Value: " << cpu.getMemory(0x09F)
                                              << "Expected " << 0xF8;
}

TEST(NAME, test_getByte)
{
    int16_t data = 0b01101010;
    ASSERT_FALSE(getByte(0, data));
    ASSERT_TRUE(getByte(1, data));
    ASSERT_TRUE(getByte(2, data));
    ASSERT_FALSE(getByte(3, data));
    ASSERT_FALSE(getByte(7, data));
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}