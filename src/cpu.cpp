#include <stdint.h>
#include <iostream>
#include "cpu.h"
#include <random>

// The font used to display numbers on screen, each set of 5 bytes is a single Digit/Number, called by a special drawer function
uint8_t font [] = {
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

void CPU::loadFile(char * filePath){

    printf("Loading ROM: %s\n", filePath);

    FILE* rom = fopen(filePath, "rb" );
    if (rom == NULL) {
        std::cout << "Failed to Open ROM" << std::endl;
        return;
    }

    // Get file size
    fseek(rom, 0, SEEK_END); // seek to end of file to find size
    long fileSize = ftell(rom);
    fseek(rom, 0, SEEK_SET); // return to beginning of file

    // Check if ROM fits in memory (4096 - 0x200)
    if (fileSize > 0x1000 - 0x200) {
        std::cout << "ROM too large for memory" << std::endl;
        fclose(rom);
        return;
    }

    // Read ROM data into memory starting at 0x200
    size_t bytesRead = fread(&RAM[0x200], 1, fileSize, rom);
    fclose(rom);
    
    if (bytesRead != fileSize) {
        std::cout << "Error reading ROM file" << std::endl;
        return;
    }
    
    std::cout << "Loaded " << bytesRead << " bytes into memory" << std::endl;

    // Load font into memory starting at 0x50
    for (int i = 0; i < sizeof(font); ++i) {
        RAM[0x50 + i] = font[i];
    }
}


void CPU::executeOpcode(uint16_t opcode) {
    uint8_t X = (opcode & 0x0F00) >> 8;
    uint8_t Y = (opcode & 0x00F0) >> 4;
    uint8_t N = opcode & 0x000F;
    uint8_t NN = opcode & 0x00FF;
    uint16_t NNN = opcode & 0x0FFF;

    switch (opcode & 0xF000) {
    case 0x0000:
        switch (opcode) {
        case 0x00E0: // 00E0
            // Clears the screen
            memset(display, 0, sizeof(display));
            break;
        case 0x00EE: // 00EE
            // Returns from a subroutine
            if (SP > 0) {
                SP--;
                PC = stack[SP];
            } else {
                std::cerr << "Stack underflow at PC=" << std::hex << PC << std::endl;
            }
            break;
        default:
        // 0NNN should be ignored for most modern emulators
            break;
        }
        break;
    case 0x1000: // 1NNN
        // Jumps to address NNN
        PC = NNN;
        PC-=2;
        break;
    case 0x2000: // 2NNN
        // Calls subroutine at NNN
        stack[SP]=PC;
        SP++;
        PC = NNN;
        PC-=2;
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
        {
            uint16_t sum = V[X] + V[Y];
            V[0xF] = ( sum > 0xFF ) ? 1 : 0; // Set VF to 1 if there's a carry
            V[X] = sum & 0xFF;
        }
            break;
        case 0x0005: // 8XY5
            // VY is subtracted from VX
            V[0xF] = (V[X] >= V[Y]) ? 1 : 0; // Set VF to 0 if there's a borrow
            V[X] -= V[Y];
            break;
        case 0x0006: // 8XY6
            // Shifts VX to the right by 1
            V[0xF] = V[X] & 0x1; // Save least significant bit in VF
            V[X] >>= 1;
            break;
        case 0x0007: // 8XY7
            // Sets VX to VY minus VX
            V[0xF] = (V[Y] >= V[X]) ? 1 : 0; // Set VF to 0 if there's a borrow
            V[X] = V[Y] - V[X];
            break;
        case 0x000E: // 8XYE
            V[0xF] = (V[X] & 0x80) >> 7; // Save most significant bit in VF
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
        PC-=2;
        break;
    case 0xC000: // CXNN
    {    // Sets VX to the result of a bitwise and operation on a random number and NN
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<> dis(0, 255);
        V[X] = dis(gen) & NN;
    }
        break;
    case 0xD000: // DXYN
        // Draws a sprite at coordinate (VX, VY) with width of 8 pixels and height of N pixels
        // We use modulo for screen wrapping at edges to happen nicely
        // x is the x coordinate, y is the y coordinate, h is the height (amount of lines), p is the current pixel.
        {
        uint8_t xCoord = V[X] % 64;
        uint8_t yCoord = V[Y] % 32;
        V[0xF]=0; // VF = 0

        for (int row = 0; row < N; row++) {
            if (yCoord >= 32) break;  // stop if we go past screen height

             // read the row of sprite data from memory
            uint8_t spriteByte = RAM[I + row];

            // draw each bit of this row
            for (int bit = 0; bit < 8; bit++) {
                if (xCoord >= 64) break;   // stop if we go past screen width

                // check if the current bit is set
                if (spriteByte & (0x80 >> bit)) {
                    // if screen pixel is on, turn it off and set VF=1
                    if (display[xCoord][yCoord]) {
                        display[xCoord][yCoord] = false;
                        V[0xF] = 1;
                    } else {
                        // otherwise, turn it on
                        display[xCoord][yCoord] = true;
                    }
                }
                ++xCoord;
            }

            // reset x for the next row, then increment y
            xCoord = V[X] % 64;
            ++yCoord;
            }
        }
        break;
    case 0xE000:
        switch (opcode & 0x00FF) {
        case 0x009E: // EX9E
            // Skips the next instruction if the key stored in VX is pressed
            if (V[X] == pressedKey){ PC+=2; }
            break;
        case 0x00A1: // EXA1
            // Skips the next instruction if the key stored in VX is not pressed
            if (V[X] != pressedKey){ PC+=2; }
            break;
        }
        break;
    case 0xF000:
        switch (opcode & 0x00FF) {
        case 0x0007: // FX07
            // Sets VX to the value of the delay timer
            V[X] = DELAY;
            break;
        case 0x000A: // FX0A
            // A key press is awaited, and then stored in VX            
            if (waitingForKeyRelease) {
                // We've captured a key press and are waiting for release
                if (pressedKey == 0xFF) {
                    // Key has been released, continue execution
                    waitingForKeyRelease = false;
                } else {
                    // Key still pressed, wait
                    PC -= 2;
                }
            } else {
                // Waiting for initial key press
                if (pressedKey != 0xFF) {
                    // Key pressed, save it
                    V[X] = pressedKey;
                    lastKey = pressedKey;
                    waitingForKeyRelease = true;
                    PC -= 2;  // Stay on this instruction
                } else {
                    // No key pressed, keep waiting
                    PC -= 2;
                }
            }
            break; 
        case 0x0015: // FX15
            // Sets the delay timer to VX
            DELAY = V[X];
            break;
        case 0x0018: // FX18
            // Sets the sound timer to VX
            TIMER = V[X];
            break;
        case 0x001E: // FX1E
            // Adds VX to I
            I += V[X];
            break;
        case 0x0029: // FX29
            // Sets I to the location of the sprite for the character in VX
            I = 0x50 + (V[X] * 5);
            break;
        case 0x0033: // FX33
            // Stores the binary-coded decimal representation of VX
            RAM[I] = V[X] / 100;
            RAM[I + 1] = (V[X] / 10) % 10;
            RAM[I + 2] = V[X] % 10;
            break;
        case 0x0055: // FX55
            // Stores from V0 to VX in memory starting at address I
            for (int i = 0; i <= X; ++i) {
                RAM[I + i] = V[i];
            }
            break;
        case 0x0065: // FX65
            // Fills from V0 to VX with values from memory starting at address I
            for (int i = 0; i <= X; ++i) {
                V[i] = RAM[I + i];
            }
            break;
        }
        break;
    default:
        // Unknown opcode
        break;
    }
}

void CPU::Cycle(){
    opcode = RAM[PC] << 8 | RAM[PC + 1]; // The Current Opcode is the OR of the 2 consecutive bytes in memory
    executeOpcode(opcode); // jump to opcode execution switch case to decode and execute opcode
    if(DELAY > 0){ DELAY--; } // decrement delay timer
    if (TIMER == 1) { system("mpg123 meow.mp3 > /dev/null 2>&1 &");} // play the defined "beep" sound, running concurrently with program
    // dev/null output redirection to prevent console from being spammed whenever the sound is decoded
    if (TIMER > 0){ TIMER--; } // decrement timer
    PC+=2; // No matter the opcode, incremnt PC by 2, logic for halting and looping implemented inside opcodes
}

void CPU::setKeyPress(uint8_t key) {
    pressedKey = key;
}
