//
// Created by michel on 13.11.21.
//

#ifndef CHIP8_MEMORY_H
#define CHIP8_MEMORY_H

#include <cstdlib>
#include <system_error>

#define MEM_SIZE 4096

class Memory {
public:
    char* memory;

public:
    Memory() {
        memory = static_cast<char *>(aligned_alloc(MEM_SIZE, MEM_SIZE));
        if (!memory) {
            throw std::system_error();
        }
    }

    virtual ~Memory() {
        free(memory);
    }

    void initDigits(void* address) {
        //TODO all 16 digit sprites should be at address 0 in the 4k memory.
    }
};

#endif //CHIP8_MEMORY_H
