//
// Created by michel on 13.11.21.
//

#ifndef CHIP8_PROCESSOR_H
#define CHIP8_PROCESSOR_H

#include <cassert>
#include "Memory.h"
#include "Display.h"
#include "CodeLoader.h"
#include "RegisterFile.h"

#include <boost/format.hpp>

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

    void runProgam() {
        while (this->registerFile.PC >= 0 && this-> registerFile.PC < this->instr->size) {
            executeInstruction();
        }
        std::cout << "Regular Termination." << std::endl;
    }

    void executeInstruction() {
        std::cout << "Instruction: " << boost::format("%02x") % +this->instr->code[registerFile.PC] << boost::format("%02x") % +this->instr->code[registerFile.PC + 1] << " at " << this->registerFile.PC << std::dec << std::endl;
        switch (this->instr->code[registerFile.PC] & LOWER_MASK) {
            case 0x00:
                handle0instr();
                break;
            case 0x10:
                handle1instr();
                break;
            case 0x20:
                handle2instr();
                break;
            default:
                std::cerr << "Unknown instruction: " << boost::format("%02x") % +this->instr->code[registerFile.PC] << boost::format("%02x") % +this->instr->code[registerFile.PC + 1] << std::endl;
                break;
        }
    }

    void dumpRegisters() {
        std::cout << "Registers:" << std::endl;
        for (int i = 0; i < 16; i++) {
            std::cout << "V" << i << ": " << +this->registerFile.V[i] << std::endl;
        }
        std::cout << "PC: " << +this->registerFile.PC <<std::endl;
        std::cout << "SP: " << +this->registerFile.SP <<std::endl;
        std::cout << "DT: " << +this->registerFile.DT <<std::endl;
        std::cout << "I: " << +this->registerFile.I <<std::endl;
        std::cout << "ST: " << +this->registerFile.ST <<std::endl;
        std::cout << "End of Registers." << std::endl;
    }

private:

    void handle0instr() {
        if (this->instr->code[this->registerFile.PC] == 0) {
            if (this->instr->code[this->registerFile.PC + 1] == 0xE0) {
                //CLS
                this->display->clear();
                this->registerFile.PC += 2;
                return;
            }
            if (this->instr->code[this->registerFile.PC + 1] == 0xEE) {
                //RET
                assert(registerFile.SP < 35 && registerFile.SP > 1); //SP is not -1 or some other overflow, and stack is not empty
                this->registerFile.SP -= 2; //since stack entries are 2 bytes long
                this->registerFile.PC = this->memory->memory[STACK_START + this->registerFile.SP + 1];
                this->registerFile.PC += ((uint16_t)this->memory->memory[STACK_START + this->registerFile.SP]) << 8;
                this->registerFile.PC += 2;
                return;
            }
            std::cerr << "Unknown instruction: " << std::hex << this->instr->code[this->registerFile.PC] << this->instr->code[this->registerFile.PC + 1] << std::dec << std::endl;
        }
    }

    void handle1instr() {
        //JP addr
        uint16_t higher = this->instr->code[this->registerFile.PC] & HIGHER_MASK;
        uint16_t lower = this->instr->code[this->registerFile.PC + 1];
        uint16_t newPC = lower + (higher << 8);
        this->registerFile.PC = newPC;
    }

    void handle2instr() {
        //CALL addr
        assert(this->registerFile.SP < 33);
        this->memory->memory[STACK_START + registerFile.SP] = getHigher(this->registerFile.PC);
        this->memory->memory[STACK_START + registerFile.SP + 1] = getLower(this->registerFile.PC); //TODO check this for endianness

        uint16_t higher = this->instr->code[this->registerFile.PC] & HIGHER_MASK;
        uint16_t lower = this->instr->code[this->registerFile.PC + 1];
        uint16_t newPC = lower + (higher << 8);
        this->registerFile.PC = newPC;

        //std::cout << this->registerFile.PC << std::endl;

        this->registerFile.SP += 2; //write first, then increment
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
