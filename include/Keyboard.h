//
// Created by michel on 15.11.21.
//

#ifndef CHIP8_KEYBOARD_H
#define CHIP8_KEYBOARD_H

class Keyboard {
public:
    /**
     * checks if given button is currently pressed
     * @param button
     * @return true if button is currently pressed
     */
    static bool isPressed(uint8_t button) {
        std::cerr << "not yet implemented" << std::endl;
        return false;
    }

    /**
     * holds all execution until a button is pressed
     * @return value of button that was pressed
     */
    static uint8_t waitForKey() {
        std::cerr << "not yet implemented" << std::endl;
        return -1;
    }
};

#endif //CHIP8_KEYBOARD_H
