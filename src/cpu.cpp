#include <stdint.h>
#include <iostream>

class CPU{

private:
// Setting up 16 general purpose 8-bit registers, mostly used for memory addresses, so only rightmost 12 bits used.
uint8_t V0,V1,V2,V3,V4,V5,V6,V7,V8,V9,VA,VB,VC,VD,VE;
// VF is special in that it's a flag, used by some instuctions
uint8_t VF; 

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

public:
CPU(){
    std::cout<<"CPU Created!"<<"\n";
}
void CPUtest(){
    std::cout<<"CPU Test Function Run!"<<"\n";
}

};