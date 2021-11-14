//
// Created by michel on 13.11.21.
//

#ifndef CHIP8_PROCESSOR_H
#define CHIP8_PROCESSOR_H

class Processor{
private:
    Display64x32* display;
    Memory* memory;
    program* instr;
public:
    Processor(Display64x32* display, Memory* memory, program* instructions) {
        this.display = display;
        this.memory = memory;
        this.instr = instructions;
    }



};

#endif //CHIP8_PROCESSOR_H
