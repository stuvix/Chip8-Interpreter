//
// Created by michel on 15.11.21.
//

#ifndef CHIP8_KEYBOARD_H
#define CHIP8_KEYBOARD_H

#include <bitset>
#include <iostream>
#include <string>
#include <algorithm>
#include <thread>
#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QGridLayout>
#include <condition_variable>

class Keyboard : public QWidget {

    Q_OBJECT

public:
    uint8_t keyDown[16] = {0};
    #define NO_PRESS (uint8_t)-1
    uint8_t lastKey = NO_PRESS;
    std::condition_variable waitForKeyDown;
    std::mutex cvM;

public:
    /**
     * checks if given button is currently pressed
     * @param button
     * @return true if button is currently pressed
     */
    bool isPressed(uint8_t button) {
        //std::cerr << "not yet implemented" << std::endl;
        //std::cout << "isPressed with " << +button << " " << +keyDown[button] << std::endl;
        return this->keyDown[button];
    }

    /**
     * holds all execution until a button is pressed
     * @return value of button that was pressed
     */
    uint8_t waitForKey() {
        //std::cerr << "not yet implemented" << std::endl;
        std::unique_lock<std::mutex> l(this->cvM);
        while (this->lastKey == NO_PRESS) {
            this->waitForKeyDown.wait(l);
        }
        uint8_t key = this->lastKey;
        this->lastKey = NO_PRESS;
        return key;
    }


public:
    Keyboard(QWidget* parent = nullptr) :
    QWidget(parent){
            auto * oneBtn = new KeyBoardButton("1", this, 1);
            auto * twoBtn = new KeyBoardButton("2", this, 2);
            auto * threeBtn = new KeyBoardButton("3", this, 3);
            auto * cBtn = new KeyBoardButton("C", this, 0xc);
            auto * fourBtn = new KeyBoardButton("4", this, 4);
            auto * fiveBtn = new KeyBoardButton("5", this, 5);
            auto * sixBtn = new KeyBoardButton("6", this, 6);
            auto * dBtn = new KeyBoardButton("D", this, 0xd);
            auto * sevenBtn = new KeyBoardButton("7", this, 7);
            auto * eightBtn = new KeyBoardButton("8", this, 8);
            auto * nineBtn = new KeyBoardButton("9", this, 9);
            auto * eBtn = new KeyBoardButton("E", this, 0xe);
            auto * aBtn = new KeyBoardButton("A", this, 0xa);
            auto * zeroBtn = new KeyBoardButton("0", this, 0);
            auto * bBtn = new KeyBoardButton("B", this, 0xb);
            auto * fBtn = new KeyBoardButton("F", this, 0xf);

            auto* grid = new QGridLayout(this);
            grid->addWidget(oneBtn, 0, 0);
            grid->addWidget(twoBtn, 0, 01);
            grid->addWidget(threeBtn, 0, 02);
            grid->addWidget(cBtn, 0, 03);
            grid->addWidget(fourBtn, 1, 0);
            grid->addWidget(fiveBtn, 1, 01);
            grid->addWidget(sixBtn, 1, 02);
            grid->addWidget(dBtn, 1, 03);
            grid->addWidget(sevenBtn, 2, 0);
            grid->addWidget(eightBtn, 2, 01);
            grid->addWidget(nineBtn, 2, 02);
            grid->addWidget(eBtn, 2, 03);
            grid->addWidget(aBtn, 3, 0);
            grid->addWidget(zeroBtn, 3, 01);
            grid->addWidget(bBtn, 3, 02);
            grid->addWidget(fBtn, 3, 03);

            setLayout(grid);

            //connect(oneBtn, &QPushButton::clicked, this, &Keyboard::onButtonDown);
    }

    ~Keyboard() {
    }


private slots:


public:
    class KeyBoardButton : public QPushButton {
        uint8_t value;
        Keyboard* outer;

        virtual void mousePressEvent(QMouseEvent* event) {
            //std::cout << +value << "down" << std::endl;
            assert(value < 16);
            outer->keyDown[value] = (uint8_t)1;
            outer->lastKey = value;
            outer->waitForKeyDown.notify_all();
        }

        virtual void mouseReleaseEvent(QMouseEvent * event) {
            //std::cout << +value << "up" << std::endl;
            assert(value < 16);
            outer->keyDown[value] = 0;
        }

    public:
        KeyBoardButton(const char string[2], Keyboard *pKeyboard, uint8_t v) : QPushButton(string, pKeyboard){
            this->value = v;
            this->outer = pKeyboard;
        }
    };
};

#endif //CHIP8_KEYBOARD_H
