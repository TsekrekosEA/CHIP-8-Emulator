#include <stdint.h>
#include <stdio.h>
#include <cstring>

class CPU{

private:
// Setting up 16 general purpose 8-bit registers, mostly used for memory addresses, so only rightmost 12 bits used. // VF is special in that it's a flag, used by some instuctions
uint8_t V[16];

// the index register
uint16_t I;

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

// current op code
uint16_t opcode,X,Y,N,NN,NNN;

// a 64x32 boolean array of the screen, True means the pixel is on, False means the pixel is off
bool display [64][32];

// stores the current key pressed
uint8_t pressedKey;
// variables needed for key press/key release logic to work
bool waitingForKeyRelease = false;
uint8_t lastKey = 0xFF;


public:
CPU(){ // CPU Constructor, Initializes all fields to their default values
    std::cout<<"CPU Created!"<<"\n";

    // traditionally 0x000 to 0x1FF was where the interpret was located, programs start at 0x200
    PC = 0x200;
    SP = 0;
    I = 0;
    DELAY = 0;
    TIMER = 0;
    opcode = 0;
    pressedKey = 0xFF;


    // Clear memory, registers, and display
    memset(RAM, 0, sizeof(RAM));
    memset(V, 0, sizeof(V));
    memset(display, 0, sizeof(display));

}

~CPU(){};

bool getPixel(int x, int y) const { return display[x][y]; }
void executeOpcode(uint16_t opcode);
void Cycle();
void loadFile(char * filePath);
void setKeyPress(uint8_t key);

};