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

    //void (Processor::*handlers[16])(void) = {};

public:
    Processor(Display64x32* display, Memory* memory, program* instructions) {
        this->display = display;
        this->memory = memory;
        this->instr = instructions;
        this->registerFile.SP = 0; //stack starts at address 0x200, yet SP is 0 if it points to 0x200

        /*this->handlers[0] = &Processor::handle0instr;
        this->handlers[1] = &Processor::handle1instr;
        this->handlers[2] = &Processor::handle2instr;
        this->handlers[3] = &Processor::handle3instr;
        this->handlers[4] = &Processor::handle4instr;
        this->handlers[5] = &Processor::handle5instr;
        this->handlers[6] = &Processor::handle6instr;
        this->handlers[7] = &Processor::handle7instr;
        this->handlers[8] = &Processor::handle8instr;
        this->handlers[9] = &Processor::handle9instr;
        this->handlers[0xa] = &Processor::handleAinstr;
        this->handlers[0xb] = &Processor::handleBinstr;
        this->handlers[0xc] = &Processor::handleCinstr;
        this->handlers[0xd] = &Processor::handleDinstr;
        this->handlers[0xe] = &Processor::handleEinstr;
        this->handlers[0xf] = &Processor::handleFinstr;*/
    }

    void runProgam() {
        while (this->registerFile.PC >= 0 && this-> registerFile.PC < this->instr->size) {
            executeInstruction();
        }
        std::cout << "Regular Termination." << std::endl;
    }

    void executeInstruction() {
        std::cout << "Instruction: " << boost::format("%02x") % +this->instr->code[registerFile.PC] << boost::format("%02x") % +this->instr->code[registerFile.PC + 1] << " at " << this->registerFile.PC << std::dec << std::endl;

        switch (this->instr->code[this->registerFile.PC >> 4]) {
            case 0:
                this->handle0instr();
                break;
            case 1:
                this->handle1instr();
                break;
            case 2:
                this->handle2instr();
                break;
            case 3:
                this->handle3instr();
                break;
            case 4:
                this->handle4instr();
                break;
            case 5:
                this->handle5instr();
                break;
            case 6:
                this->handle6instr();
                break;
            case 7:
                this->handle7instr();
                break;
            case 8:
                this->handle8instr();
                break;
            case 9:
                this->handle9instr();
                break;
            case 0xa:
                this->handleAinstr();
                break;
            case 0xb:
                this->handleBinstr();
                break;
            case 0xc:
                this->handleCinstr();
                break;
            case 0xd:
                this->handleDinstr();
                break;
            case 0xe:
                this->handleEinstr();
                break;
            case 0xf:
                this->handleFinstr();
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
                assert(registerFile.SP < 35);
                assert(registerFile.SP > 1); //SP is not -1 or some other overflow, and stack is not empty
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

        this->registerFile.SP += 2; //write first, then increment
    }

    void handle3instr() {
        //SE Vx, byte - skip if equal
        uint8_t regNumber = this->instr->code[this->registerFile.PC] & HIGHER_MASK;
        uint8_t kk = this->instr->code[this->registerFile.PC + 1];
        if (this->registerFile.V[regNumber] == kk) {
            //skip next
            this->registerFile.PC += 2;
        }
        else {
            //don't skip
            this->registerFile.PC += 1;
        }
    }

    void handle4instr() {
        //SNE Vx, byte - skip if not equal
        uint8_t regNumber = this->instr->code[this->registerFile.PC] & HIGHER_MASK;
        uint8_t kk = this->instr->code[this->registerFile.PC + 1];
        if (this->registerFile.V[regNumber] != kk) {
            //skip next
            this->registerFile.PC += 2;
        }
        else {
            //don't skip
            this->registerFile.PC += 1;
        }
    }

    void handle5instr() {
        //SE Vx, Vy
        uint8_t regNumber1 = this->instr->code[this->registerFile.PC] & HIGHER_MASK;
        uint8_t regNumber2 = this->instr->code[this->registerFile.PC + 1] >> 4;
        if (this->registerFile.V[regNumber1] == this->registerFile.V[regNumber2]) {
            //skip next
            this->registerFile.PC += 2;
        }
        else {
            //don't skip
            this->registerFile.PC += 1;
        }
    }

    void handle6instr() {
        //LD Vx, byte
        uint8_t regNumber = this->instr->code[this->registerFile.PC] & HIGHER_MASK;
        uint8_t kk = this->instr->code[this->registerFile.PC + 1];
        this->registerFile.V[regNumber] = kk;
        this->registerFile.PC += 1;
    }

    void handle7instr() {
        //ADD Vx, byte
        uint8_t regNumber = this->instr->code[this->registerFile.PC] & HIGHER_MASK;
        uint8_t kk = this->instr->code[this->registerFile.PC + 1];
        this->registerFile.V[regNumber] += kk;
        this->registerFile.PC += 1;
    }

    void handle8instr() {
        uint8_t lastByte = this->instr->code[this->registerFile.PC + 1] & HIGHER_MASK;
        uint8_t regNumber1 = this->instr->code[this->registerFile.PC] & HIGHER_MASK;
        uint8_t regNumber2 = this->instr->code[this->registerFile.PC + 1] >> 4;
        switch (lastByte) {
            case 0:
                //LD Vx, Vy
                this->registerFile.V[regNumber1] = this->registerFile.V[regNumber2];
                break;
            case 1:
                //OR Vx, Vy
                this->registerFile.V[regNumber1] |= this->registerFile.V[regNumber2];
                break;
            case 2:
                //AND Vx, Vy
                this->registerFile.V[regNumber1] &= this->registerFile.V[regNumber2];
                break;
            case 3:
                //XOR Vx, Vy
                this->registerFile.V[regNumber1] ^= this->registerFile.V[regNumber2];
                break;
            case 4:
            {
                //ADD Vx, Vy
                uint16_t largerSum = (uint16_t)this->registerFile.V[regNumber1] + (uint16_t)this->registerFile.V[regNumber2];
                this->registerFile.V[regNumber1] += this->registerFile.V[regNumber2];
                if (largerSum != this->registerFile.V[regNumber1]) {
                    //overflow, set carry
                    this->registerFile.V[0xF] = 1;
                }
                else {
                    this->registerFile.V[0xF] = 0;
                }
                break;
            }
            case 5:
                //SUB Vx, Vy
                if (this->registerFile.V[regNumber1] > this->registerFile.V[regNumber2]) {
                    this->registerFile.V[0xF] = 1;
                }
                else {
                    this->registerFile.V[0xF] = 0;
                }
                this->registerFile.V[regNumber1] -= this->registerFile.V[regNumber2];
                break;
            case 6:
                //SHR Vx {, Vy}
                this->registerFile.V[0xF] = this->registerFile.V[regNumber1] & 0x1;
                this->registerFile.V[0xF] >>= 1;
                break;
            case 7:
                //SUBN Vx, Vy : Vx = Vy - Vx
                if (this->registerFile.V[regNumber1] < this->registerFile.V[regNumber2]) {
                    this->registerFile.V[0xF] = 1;
                }
                else {
                    this->registerFile.V[0xF] = 0;
                }
                this->registerFile.V[regNumber1] = this->registerFile.V[regNumber2] - this->registerFile.V[regNumber1];
                break;
            case 0xE:
                //SHL Vx {, Vy}
                this->registerFile.V[0xF] = this->registerFile.V[regNumber1] & 0x80;
                this->registerFile.V[0xF] <<= 2;
                break;
            default:
                std::cerr << "Unknown instruction" << std::endl;
        }
        this->registerFile.PC += 1;
    }

    void handle9instr() {
        //SNE Vx, Vy
        uint8_t regNumber1 = this->instr->code[this->registerFile.PC] & HIGHER_MASK;
        uint8_t regNumber2 = this->instr->code[this->registerFile.PC + 1] >> 4;
        if (this->registerFile.V[regNumber1] != this->registerFile.V[regNumber2]) {
            this->registerFile.PC += 2;
        }
        else {
            this->registerFile.PC += 1;
        }
    }

    void handleAinstr() {
        //LD I, 12bits
        uint16_t i = this->instr->code[this->registerFile.PC + 1];
        i += ((uint16_t)this->instr->code[this->registerFile.PC] & 0xF) << 8;
        this->registerFile.I = i;
        this->registerFile.PC += 1;
    }

    void handleBinstr() {
        //JP V0 + addr
        uint16_t i = this->instr->code[this->registerFile.PC + 1];
        i += ((uint16_t)this->instr->code[this->registerFile.PC] & 0xF) << 8;
        i += this->registerFile.V[0x0];
        this->registerFile.PC = i;
    }

    void handleCinstr() {
        //RND Vx, byte
        uint8_t regNumber = this->instr->code[this->registerFile.PC] & HIGHER_MASK;
        uint8_t kk = this->instr->code[this->registerFile.PC + 1];
        this->registerFile.V[regNumber] = ((uint8_t)random()) & kk;
        this->registerFile.PC += 1;
    }

    void handleDinstr() {
        //DRW Vx, Vy, nibble
        uint8_t regNumber1 = this->instr->code[this->registerFile.PC] & HIGHER_MASK;
        uint8_t regNumber2 = this->instr->code[this->registerFile.PC + 1] >> 4;
        uint8_t nibble = this->instr->code[this->registerFile.PC] & HIGHER_MASK;
        if (this->display->addSprite(this->registerFile.V[regNumber1], this->registerFile.V[regNumber2], this->instr->code + this->registerFile.I, nibble)) {
            this->registerFile.V[0xF] = 1;
        }
        else {
            this->registerFile.V[0xF] = 0;
        }
        this->registerFile.PC += 1;
    }

    void handleEinstr() {

    }

    void handleFinstr() {

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
