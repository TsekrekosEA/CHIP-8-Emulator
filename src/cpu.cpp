#include <stdint.h>
#include <iostream>
#include "cpu.h"


void CPU::CPUtest(){
    std::cout<<"CPU Test Function Run!"<<"\n";
}


void CPU::executeOpcode(uint16_t opcode) {
    uint8_t X = (opcode & 0x0F00) >> 8;
    uint8_t Y = (opcode & 0x00F0) >> 4;
    uint8_t N = opcode & 0x000F;
    uint8_t NN = opcode & 0x00FF;
    uint16_t NNN = opcode & 0x0FFF;

    switch (opcode) {
    case 0x00E0: // 00E0
        // Clears the screen
        for(int i=0;i<64;i++){
            for(int j=0;j<32;j++){
                display[i][j]=false;
            }
        }
        break;
    case 0x00EE: // 00EE
        // Returns from a subroutine
        SP--;
        PC=stack[SP];
        break;
    default:
        switch (opcode & 0xF000) {
        case 0x0000: // 0NNN
            // Calls machine code routine at address NNN
            break;
        case 0x1000: // 1NNN
            // Jumps to address NNN
            PC = NNN;
            break;
        case 0x2000: // 2NNN
            // Calls subroutine at NNN
            stack[SP]=PC;
            SP++;
            PC = NNN;
            break;
        case 0x3000: // 3XNN
            // Skips the next instruction if VX equals NN
            if( V[X] == NN){ PC+=2; }
            break;
        case 0x4000: // 4XNN
            // Skips the next instruction if VX does not equal NN
            if( V[X] != NN){ PC+=2; }
            break;
        case 0x5000: // 5XY0
            // Skips the next instruction if VX equals VY
            if( V[X] == V[Y]){ PC+=2; }
            break;
        case 0x6000: // 6XNN
            // Sets VX to NN
            V[X] = NN;
            break;
        case 0x7000: // 7XNN
            // Adds NN to VX
            V[X] += NN;
            break;
        case 0x8000:
            switch (N) {
            case 0x0000: // 8XY0
                // Sets VX to the value of VY
                V[X] = V[Y];
                break;
            case 0x0001: // 8XY1
                // Sets VX to VX or VY
                V[X] = V[X] | V[Y];
                break;
            case 0x0002: // 8XY2
                // Sets VX to VX and VY
                V[X] = V[X] & V[Y];
                break;
            case 0x0003: // 8XY3
                // Sets VX to VX xor VY
                V[X] = V[X] ^ V[Y];
                break;
            case 0x0004: // 8XY4
                // Adds VY to VX
                V[X] += V[Y];
                break;
            case 0x0005: // 8XY5
                // VY is subtracted from VX
                V[X] -= V[Y];
                break;
            case 0x0006: // 8XY6
                // Shifts VX to the right by 1
                V[X] >>= 1;
                break;
            case 0x0007: // 8XY7
                // Sets VX to VY minus VX
                V[X] = V[Y] - V[X];
                break;
            case 0x000E: // 8XYE
                // Shifts VX to the left by 1
                V[X] <<= 1;
                break;
            }
            break;
        case 0x9000: // 9XY0
            // Skips the next instruction if VX does not equal VY
            if( V[X] != V[Y]){ PC+=2; }
            break;
        case 0xA000: // ANNN
            // Sets I to the address NNN
            I = NNN;
            break;
        case 0xB000: // BNNN
            // Jumps to the address NNN plus V0
            PC = NNN + V[0];
            break;
        case 0xC000: // CXNN
            // Sets VX to the result of a bitwise and operation on a random number and NN
            V[X] = (rand() % 256) & NN;
            break;
        case 0xD000: // DXYN
            // Draws a sprite at coordinate (VX, VY) with width of 8 pixels and height of N pixels
            // Implementation of drawing sprite
            break;
        case 0xE09E: // EX9E
            // Skips the next instruction if the key stored in VX is pressed
            // Implementation of key press check
            break;
        case 0xE0A1: // EXA1
            // Skips the next instruction if the key stored in VX is not pressed
            // Implementation of key press check
            break;
        case 0xF007: // FX07
            // Sets VX to the value of the delay timer
            V[X] = DELAY;
            break;
        case 0xF00A: // FX0A
            // A key press is awaited, and then stored in VX
            // Implementation of key press wait
            break;
        case 0xF015: // FX15
            // Sets the delay timer to VX
            DELAY = V[X];
            break;
        case 0xF018: // FX18
            // Sets the sound timer to VX
            TIMER = V[X];
            break;
        case 0xF01E: // FX1E
            // Adds VX to I
            I += V[X];
            break;
        case 0xF029: // FX29
            // Sets I to the location of the sprite for the character in VX
            // Implementation of setting I to sprite location
            break;
        case 0xF033: // FX33
            // Stores the binary-coded decimal representation of VX
            // Implementation of BCD representation
            break;
        case 0xF055: // FX55
            // Stores from V0 to VX in memory starting at address I
            // Implementation of storing in memory
            break;
        case 0xF065: // FX65
            // Fills from V0 to VX with values from memory starting at address I
            // Implementation of filling from memory
            break;
        default:
            // Unknown opcode
            break;
        }
        break;
    }
}

void CPU::Cycle(){
    opcode = RAM[PC] << 8 | RAM[PC + 1];
    executeOpcode(opcode);
    if(DELAY > 0){ DELAY--; }
    if (TIMER == 1) {
        // implement sound
    }
    if (TIMER > 0){ TIMER--; }
    PC+=2;
}

