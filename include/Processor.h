//
// Created by michel on 13.11.21.
//

#ifndef CHIP8_PROCESSOR_H
#define CHIP8_PROCESSOR_H

#include "Memory.h"
#include "Display.h"
#include "CodeLoader.h"
#include "RegisterFile.h"

#define LOWER_MASK 0xF0
#define HIGHER_MASK 0x0F

#define STACK_START 0x200

class Processor{
private:
    Display64x32* display{};
    Memory* memory{};
    program* instr{};
    RegisterFile registerFile = {0};

public:
    Processor(Display64x32* display, Memory* memory, program* instructions) {
        this->display = display;
        this->memory = memory;
        this->instr = instructions;
        this->registerFile.SP = 0; //stack starts at address 0x200, yet SP is 0 if it points to 0x200
    }

    void executeInstruction() {
        switch (this->instr[registerFile.PC] & LOWER_MASK) {
            case 0x00:
                handle0instr();
                break;
            case 0x10:
                handle1instr();
                break;
        }
    }

    void handle0instr() {
        if (this->instr[this->registerFile.PC] == 0) {
            if (this->instr[this->registerFile.PC + 1] == 0xE0) {
                //CLS
                this->display->clear();
                this->registerFile.PC += 2;
                return;
            }
            if (this->instr[this->registerFile.PC + 1] == 0xEE) {
                //RET
                static_assert(registerFile.SP < 33 && registerFile.SP > 1); //SP is not -1 or some other overflow, and stack is not empty
                this->registerFile.PC = this->memory->memory[STACK_START + this->registerFile.SP];
                this->registerFile.SP -= 2; //since stack entries are 2 bytes long
                return;
            }
            std::cerr << "Unknown instruction: " << std::hex << this->instr[this->registerFile.PC] << this->instr[this->registerFile.PC + 1] << std::dec << std::endl;
        }
    }

    void handle1instr() {
        uint16_t higher = this->instr[this->registerFile.PC] & HIGHER_MASK;
        uint16_t lower = this->instr[this->registerFile.PC + 1];
        uint16_t newPC = lower + (higher << 8);
        this->registerFile.PC = newPC;
    }

    void handle2instr() {
        static_assert(this->registerFile.SP < 31);
        this->registerFile.SP += 2;
        this->memory->memory[STACK_START + registerFile.SP] = getHigher(this->registerFile.PC);
        this->memory->memory[STACK_START + registerFile.SP + 1] = getLower(this->registerFile.PC);

        uint16_t higher = this->instr[this->registerFile.PC] & HIGHER_MASK;
        uint16_t lower = this->instr[this->registerFile.PC + 1];
        uint16_t newPC = lower + (higher << 8);
        this->registerFile.PC = newPC;
    }


    static uint8_t getHigher(uint16_t twobyte) {
        uint16_t result = twobyte >> 8;
        return result;
    }

    static uint8_t getLower(uint16_t twobyte) {
        return twobyte;
    }

};

#endif //CHIP8_PROCESSOR_H
