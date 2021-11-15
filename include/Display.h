//
// Created by michel on 13.11.21.
//

#ifndef CHIP8_DISPLAY_H
#define CHIP8_DISPLAY_H

#include <bitset>
#include <iostream>
#include <string>
#include <algorithm>

class Display64x32 {
private:
    uint64_t array[32] = {0};

public:

    void clear() {
        for (int i = 0; i < 32; ++i) {
            array[i] = 0;
        }
    }

    void printToConsole(){
        for (int i = 0; i < 32; i++) {
            std::string line = std::bitset<64>(array[i]).to_string();
            //std::reverse(line.begin(), line.end()); //no, everything else should be reversed.
            std::cout << line << std::endl;
        }
    }

    bool addSprite(uint8_t x, uint8_t y, uint8_t* sprite, uint8_t spriteSize) {
        bool collision = false;

        std::cerr << "not yet implemented - addSprite" << std::endl; //TODO

        return collision; //true if a collision happens
    }
};

#endif //CHIP8_DISPLAY_H
