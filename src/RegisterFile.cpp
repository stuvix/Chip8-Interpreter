//
// Created by michel on 20.11.21.
//

#include "../include/RegisterFile.h"


    void decrThread::decrementPermanently(RegisterFile* registers, const bool* terminate) {
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

