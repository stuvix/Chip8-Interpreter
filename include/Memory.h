//
// Created by michel on 13.11.21.
//

#ifndef CHIP8_MEMORY_H
#define CHIP8_MEMORY_H

#include <cstdlib>
#include <system_error>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>

#define MEM_SIZE 4096
#define FONT_FILE "../res/digits.font"

class Memory {
public:
    uint8_t * memory;

public:
    Memory() {
        memory = static_cast<uint8_t *>(aligned_alloc(MEM_SIZE, MEM_SIZE));
        if (!memory) {
            throw std::system_error();
        }
    }

    virtual ~Memory() {
        free(memory);
    }

    void initDigits() {
        //TODO all 16 digit sprites should be at address 0 in the 4k memory.
        int fd = open(FONT_FILE, 0);
        if (fd == -1) {
            std::cerr << "Error opening font file" << std::endl;
            throw std::system_error();
        }
        auto* buf = new char[9];
        buf[8] = '\0';
        for (int i = 0; i < 5*16; i++) {
            if (read(fd, buf, 8) != 8) {
                std::cerr << "read fewer bytes than expected" << std::endl;
                close(fd);
                throw std::system_error();
            }
            uint8_t byte = std::stoi(buf, nullptr, 2);
            this->memory[i] = byte;
        }
        close(fd);
    }

    void dumpMemory(const char *filename) const {
        int fd = open(filename, O_CREAT | O_WRONLY, S_IRWXU | S_IRWXG | S_IRWXO);
        if (fd == -1) {
            std::cerr << "failed to open file for dumping of memory" << std::endl;
            throw std::system_error();
        }
        size_t b;
        if ((b = write(fd, memory, MEM_SIZE)) != MEM_SIZE) {
            std::cerr << "wrote wrong number of bytes while dumping memory: " << b << " errno: " << errno << std::endl;
        }
        close(fd);
    }
};

#endif //CHIP8_MEMORY_H
