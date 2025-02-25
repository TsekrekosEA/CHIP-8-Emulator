#include <stdint.h>
#include <iostream>


class CPU{

private:
// Setting up 16 general purpose 8-bit registers, mostly used for memory addresses, so only rightmost 12 bits used.
uint8_t V0,V1,V2,V3,V4,V5,V6,V7,V8,V9,VA,VB,VC,VD,VE;
// VF is special in that it's a flag, used by some instuctions
uint8_t VF; 


// the index register
uint8_t I;

// timer and delay registers for timings
uint8_t TIMER,DELAY;

// program counter, used to store current executing stack
uint16_t PC;
// stack pointer, used to point to the topmost level of the stack
uint8_t SP;

// array of 16 16-bit values, used to store the address that the interpreter should return to when finished with a subroutine.
uint16_t stack [16] = {
0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00
};

uint8_t RAM [4096];

// a 64x32 boolean array of the screen, True means the pixel is on, False means the pixel is off
bool display [64][32];



public:
CPU(){
    std::cout<<"CPU Created!"<<"\n";
    opcode_00E0(); // clear screen

}

void CPUtest();
void executeOpcode(uint16_t opcode);

// CPU opcode functions
void opcode_0NNN(); // Calls machine code routine at address NNN
void opcode_00E0(); // Clears the screen
void opcode_00EE(); // Returns from a subroutine
void opcode_1NNN(); // Jumps to address NNN
void opcode_2NNN(); // Calls subroutine at NNN
void opcode_3XNN(); // Skips the next instruction if VX equals NN
void opcode_4XNN(); // Skips the next instruction if VX does not equal NN
void opcode_5XY0(); // Skips the next instruction if VX equals VY
void opcode_6XNN(); // Sets VX to NN
void opcode_7XNN(); // Adds NN to VX
void opcode_8XY0(); // Sets VX to the value of VY
void opcode_8XY1(); // Sets VX to VX or VY
void opcode_8XY2(); // Sets VX to VX and VY
void opcode_8XY3(); // Sets VX to VX xor VY
void opcode_8XY4(); // Adds VY to VX
void opcode_8XY5(); // VY is subtracted from VX
void opcode_8XY6(); // Shifts VX to the right by 1
void opcode_8XY7(); // Sets VX to VY minus VX
void opcode_8XYE(); // Shifts VX to the left by 1
void opcode_9XY0(); // Skips the next instruction if VX does not equal VY
void opcode_ANNN(); // Sets I to the address NNN
void opcode_BNNN(); // Jumps to the address NNN plus V0
void opcode_CXNN(); // Sets VX to the result of a bitwise and operation on a random number and NN
void opcode_DXYN(); // Draws a sprite at coordinate (VX, VY) with width of 8 pixels and height of N pixels
void opcode_EX9E(); // Skips the next instruction if the key stored in VX is pressed
void opcode_EXA1(); // Skips the next instruction if the key stored in VX is not pressed
void opcode_FX07(); // Sets VX to the value of the delay timer
void opcode_FX0A(); // A key press is awaited, and then stored in VX
void opcode_FX15(); // Sets the delay timer to VX
void opcode_FX18(); // Sets the sound timer to VX
void opcode_FX1E(); // Adds VX to I
void opcode_FX29(); // Sets I to the location of the sprite for the character in VX
void opcode_FX33(); // Stores the binary-coded decimal representation of VX
void opcode_FX55(); // Stores from V0 to VX in memory starting at address I
void opcode_FX65(); // Fills from V0 to VX with values from memory starting at address I


};