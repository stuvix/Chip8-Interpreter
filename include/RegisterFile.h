//
// Created by michel on 13.11.21.
//

#ifndef CHIP8_REGISTERFILE_H
#define CHIP8_REGISTERFILE_H

struct RegisterFile{
    uint8_t V[16]; //usually called Vx, where x is the index into the array
    uint16_t I;
    uint8_t DT; //activates when reaching 0, decr@60Hz
    uint8_t ST; //emits sound while nonzero, decr@60Hz
    uint16_t PC;  //pseudo
    uint8_t SP;  //pseudo - stack located at 0x200, points to first empty slot on stack.
};

#endif //CHIP8_REGISTERFILE_H
