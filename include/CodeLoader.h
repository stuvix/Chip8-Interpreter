//
// Created by michel on 13.11.21.
//

#ifndef CHIP8_CODELOADER_H
#define CHIP8_CODELOADER_H

#include <cstdint>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <fcntl.h>
#include <unistd.h>

struct program{
    size_t size;
    uint8_t* code;
};

class CodeLoader {
    public:
        static program* loadProgram(const char* fileName) {
            auto* prog = static_cast<program *>(malloc(sizeof(program)));
            prog->size = std::filesystem::file_size(fileName);
            prog->code = static_cast<uint8_t *>(aligned_alloc(2, prog->size));

            int fd = open(fileName, 0);
            if (fd == -1) {
                throw std::system_error();
            }
            ssize_t bytesRead = read(fd, prog->code, prog->size);
            if (bytesRead == prog->size) {
                return prog;
            }
            else {
               std::cout << "read only " << bytesRead << " Bytes." << std::endl;
               throw std::system_error();
            }
        }

        static void dumpCode(program* code) {
            for (int i = 0; i < code->size / 2; ++i) {
                std::cout << std::hex << ((uint16_t*)(code->code))[i] << std::dec << std::endl;
            }
        }
};

#endif //CHIP8_CODELOADER_H
