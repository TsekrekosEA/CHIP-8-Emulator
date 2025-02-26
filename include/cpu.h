#include <stdint.h>
#include <iostream>


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

uint8_t pressedKey;


public:
CPU(){
    std::cout<<"CPU Created!"<<"\n";
    executeOpcode(0x00E0); // clear screen
    // traditionally 0x000 to 0x1FF was where the interpret was located, programs start at 0x200
    PC = 0x200;
    SP,I,opcode,DELAY,TIMER = 0;

}

~CPU(){};

bool getPixel(int x, int y) const { return display[x][y]; }
void CPUtest();
void executeOpcode(uint16_t opcode);
void Cycle();
void loadFile(char * filePath);
void setKeyPress(uint8_t key);

};