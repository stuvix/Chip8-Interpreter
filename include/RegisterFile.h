//
// Created by michel on 13.11.21.
//

#ifndef CHIP8_REGISTERFILE_H
#define CHIP8_REGISTERFILE_H

#include <thread>

struct RegisterFile{
    uint8_t V[16]; //usually called Vx, where x is the index into the array
    uint16_t I;
    uint8_t DT; //activates when reaching 0, decr@60Hz
    uint8_t ST; //emits sound while nonzero, decr@60Hz
    uint16_t PC;  //pseudo
    uint8_t SP;  //pseudo - stack located at 0x200, points to first empty slot on stack.
};

    void decrementPermanently(RegisterFile* registers, const bool* terminate) {
        while (!*terminate) {
            uint8_t DT = registers->DT;
            if (DT != 0) {
                __atomic_compare_exchange_n(&registers->DT, &DT, DT-1, false, __ATOMIC_RELAXED, __ATOMIC_RELAXED);
            }
            uint8_t ST = registers->ST;
            if (ST != 0) {
                __atomic_compare_exchange_n(&registers->ST, &ST, ST-1, false, __ATOMIC_RELAXED, __ATOMIC_RELAXED);
            }
            auto lastOperation = std::chrono::system_clock::now();
            std::this_thread::sleep_for(std::chrono::milliseconds(1000/60));
            if (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - lastOperation).count() < 1000 / 60) {
                  std::cerr << "Slept only " << std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - lastOperation).count() << "ms" << std::endl;
            }
        }
    }

#endif //CHIP8_REGISTERFILE_H
