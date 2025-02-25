#include <stdint.h>

char castKeyboardToCHIP8(char keyPress){
    switch (keyPress) {
        case '1': // Map '1' key to CHIP-8 key 1
            return '1';
        case '2': // Map '2' key to CHIP-8 key 2
            return '2';
        case '3': // Map '3' key to CHIP-8 key 3
            return '3';
        case '4': // Map '4' key to CHIP-8 key C
            return 'C';
        case 'Q': // Map 'Q' key to CHIP-8 key 4
            return '4';
        case 'W': // Map 'W' key to CHIP-8 key 5
            return '5';
        case 'E': // Map 'E' key to CHIP-8 key 6
            return '6';
        case 'R': // Map 'R' key to CHIP-8 key D
            return 'D';
        case 'A': // Map 'A' key to CHIP-8 key 7
            return '7';
        case 'S': // Map 'S' key to CHIP-8 key 8
            return '8';
        case 'D': // Map 'D' key to CHIP-8 key 9
            return '9';
        case 'F': // Map 'F' key to CHIP-8 key E
            return 'E';
        case 'Z': // Map 'Z' key to CHIP-8 key A
            return 'A';
        case 'X': // Map 'X' key to CHIP-8 key 0
            return '0';
        case 'C': // Map 'C' key to CHIP-8 key B
            return 'B';
        case 'V': // Map 'V' key to CHIP-8 key F
            return 'F';
        default: // Do nothing if key press is invalid
            return '\0'; // Return null character for invalid key press
    }
}

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