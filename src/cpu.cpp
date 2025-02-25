#include <stdint.h>
#include <iostream>
#include "cpu.h"


void CPU::CPUtest(){
    std::cout<<"CPU Test Function Run!"<<"\n";
}


// Implementations of each opcode function
void CPU::opcode_0NNN() {
    // Calls machine code routine at address NNN
}

void CPU::opcode_00E0() {
    // Clears the screen
    for(int i=0;i<64;i++){
        for(int j=0;j<32;j++){
            display[i][j]=false;
        }
    }
}

void CPU::opcode_00EE() {
    // Returns from a subroutine
}

void CPU::opcode_1NNN() {
    // Jumps to address NNN
}

void CPU::opcode_2NNN() {
    // Calls subroutine at NNN
}

void CPU::opcode_3XNN() {
    // Skips the next instruction if VX equals NN
}

void CPU::opcode_4XNN() {
    // Skips the next instruction if VX does not equal NN
}

void CPU::opcode_5XY0() {
    // Skips the next instruction if VX equals VY
}

void CPU::opcode_6XNN() {
    // Sets VX to NN
}

void CPU::opcode_7XNN() {
    // Adds NN to VX
}

void CPU::opcode_8XY0() {
    // Sets VX to the value of VY
}

void CPU::opcode_8XY1() {
    // Sets VX to VX or VY
}

void CPU::opcode_8XY2() {
    // Sets VX to VX and VY
}

void CPU::opcode_8XY3() {
    // Sets VX to VX xor VY
}

void CPU::opcode_8XY4() {
    // Adds VY to VX
}

void CPU::opcode_8XY5() {
    // VY is subtracted from VX
}

void CPU::opcode_8XY6() {
    // Shifts VX to the right by 1
}

void CPU::opcode_8XY7() {
    // Sets VX to VY minus VX
}

void CPU::opcode_8XYE() {
    // Shifts VX to the left by 1
}

void CPU::opcode_9XY0() {
    // Skips the next instruction if VX does not equal VY
}

void CPU::opcode_ANNN() {
    // Sets I to the address NNN
}

void CPU::opcode_BNNN() {
    // Jumps to the address NNN plus V0
}

void CPU::opcode_CXNN() {
    // Sets VX to the result of a bitwise and operation on a random number and NN
}

void CPU::opcode_DXYN() {
    // Draws a sprite at coordinate (VX, VY) with width of 8 pixels and height of N pixels
}

void CPU::opcode_EX9E() {
    // Skips the next instruction if the key stored in VX is pressed
}

void CPU::opcode_EXA1() {
    // Skips the next instruction if the key stored in VX is not pressed
}

void CPU::opcode_FX07() {
    // Sets VX to the value of the delay timer
}

void CPU::opcode_FX0A() {
    // A key press is awaited, and then stored in VX
}

void CPU::opcode_FX15() {
    // Sets the delay timer to VX
}

void CPU::opcode_FX18() {
    // Sets the sound timer to VX
}

void CPU::opcode_FX1E() {
    // Adds VX to I
}

void CPU::opcode_FX29() {
    // Sets I to the location of the sprite for the character in VX
}

void CPU::opcode_FX33() {
    // Stores the binary-coded decimal representation of VX
}

void CPU::opcode_FX55() {
    // Stores from V0 to VX in memory starting at address I
}

void CPU::opcode_FX65() {
    // Fills from V0 to VX with values from memory starting at address I
}


void CPU::executeOpcode(uint16_t opcode) {
    switch (opcode & 0xF000) {
    case 0x0000:
        switch (opcode & 0x00FF) {
        case 0x00E0: // 00E0
            opcode_00E0();
            break;
        case 0x00EE: // 00EE
            opcode_00EE();
            break;
        default:     // 0NNN
            opcode_0NNN();
            break;
        }
        break;
    case 0x1000: // 1NNN
        opcode_1NNN();
        break;
    case 0x2000: // 2NNN
        opcode_2NNN();
        break;
    case 0x3000: // 3XNN
        opcode_3XNN();
        break;
    case 0x4000: // 4XNN
        opcode_4XNN();
        break;
    case 0x5000: // 5XY0
        opcode_5XY0();
        break;
    case 0x6000: // 6XNN
        opcode_6XNN();
        break;
    case 0x7000: // 7XNN
        opcode_7XNN();
        break;
    case 0x8000:
        switch (opcode & 0x000F) {
        case 0x0000: // 8XY0
            opcode_8XY0();
            break;
        case 0x0001: // 8XY1
            opcode_8XY1();
            break;
        case 0x0002: // 8XY2
            opcode_8XY2();
            break;
        case 0x0003: // 8XY3
            opcode_8XY3();
            break;
        case 0x0004: // 8XY4
            opcode_8XY4();
            break;
        case 0x0005: // 8XY5
            opcode_8XY5();
            break;
        case 0x0006: // 8XY6
            opcode_8XY6();
            break;
        case 0x0007: // 8XY7
            opcode_8XY7();
            break;
        case 0x000E: // 8XYE
            opcode_8XYE();
            break;
        }
        break;
    case 0x9000: // 9XY0
        opcode_9XY0();
        break;
    case 0xA000: // ANNN
        opcode_ANNN();
        break;
    case 0xB000: // BNNN
        opcode_BNNN();
        break;
    case 0xC000: // CXNN
        opcode_CXNN();
        break;
    case 0xD000: // DXYN
        opcode_DXYN();
        break;
    case 0xE000:
        switch (opcode & 0x00FF) {
        case 0x009E: // EX9E
            opcode_EX9E();
            break;
        case 0x00A1: // EXA1
            opcode_EXA1();
            break;
        }
        break;
    case 0xF000:
        switch (opcode & 0x00FF) {
        case 0x0007: // FX07
            opcode_FX07();
            break;
        case 0x000A: // FX0A
            opcode_FX0A();
            break;
        case 0x0015: // FX15
            opcode_FX15();
            break;
        case 0x0018: // FX18
            opcode_FX18();
            break;
        case 0x001E: // FX1E
            opcode_FX1E();
            break;
        case 0x0029: // FX29
            opcode_FX29();
            break;
        case 0x0033: // FX33
            opcode_FX33();
            break;
        case 0x0055: // FX55
            opcode_FX55();
            break;
        case 0x0065: // FX65
            opcode_FX65();
            break;
        }
        break;
    default:
        // Unknown opcode
        break;
    }
}
